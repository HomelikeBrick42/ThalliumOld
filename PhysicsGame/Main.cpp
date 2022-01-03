#include "Window.hpp"
#include "Renderer.hpp"
#include "Clock.hpp"

#include "ObjLoader.hpp"

#include "Transform.hpp"

#include <cmath>

#include <iostream>
#include <format>
#include <memory>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

struct Mesh {
    Ref<VertexBuffer> VertexBuffer;
    Ref<IndexBuffer> IndexBuffer;
};

struct Point {
    glm::vec3 Position;
    glm::vec3 PreviousPosition;
    float Radius;
    glm::vec4 Color;
};

struct Stick {
    Point* A;
    Point* B;
    float Length;
    glm::vec4 Color;
};

struct Cube {
    Transform Transform;
    glm::vec4 Color;
};

static VertexBuffer::Element VertexLayout[] = {
    { VertexBuffer::ElementType::Float3, offsetof(Vertex, Position), sizeof(Vertex), false },
    { VertexBuffer::ElementType::Float3, offsetof(Vertex, Normal), sizeof(Vertex), false },
};

Mesh ObjToMesh(Ref<Renderer> renderer, const Obj::Object& object) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t index = 0;
    for (auto& face : object.Faces) {
        vertices.push_back({ .Position = object.Positions[face.Positions[0]], .Normal = object.Normals[face.Normals[0]] });
        indices.push_back(index++);
        vertices.push_back({ .Position = object.Positions[face.Positions[1]], .Normal = object.Normals[face.Normals[1]] });
        indices.push_back(index++);
        vertices.push_back({ .Position = object.Positions[face.Positions[2]], .Normal = object.Normals[face.Normals[2]] });
        indices.push_back(index++);
    }
    return {
        renderer->CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), VertexLayout),
        renderer->CreateIndexBuffer(indices),
    };
}

struct Hit {
    bool Hit;
    Point* Point;
    glm::vec3 Position;
};

Hit RaycastPoint(const std::vector<Point*>& points, const glm::vec3& position, const glm::vec3& direction) {
    for (auto& point : points) {
        // Taken from: https://www.ccs.neu.edu/home/fell/CS4300/Lectures/Ray-TracingFormulas.pdf
        float a = glm::dot(direction, direction);
        float b = 2.0f * direction.x * (position.x - point->Position.x) + 2.0f * direction.y * (position.y - point->Position.y) +
                  2.0f * direction.z * (position.z - point->Position.z);
        float c = glm::dot(point->Position, point->Position) + glm::dot(position, position) +
                  -2.0f * glm::dot(point->Position, position) - point->Radius * point->Radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant > 0) {
            float t = (-b * glm::sqrt(discriminant)) / (2.0f * a);
            if (t > 0) {
                return {
                    true,
                    point,
                    position + direction * t,
                };
            }
        }
    }
    return {};
}

int main(int, char**) {
    Ref<Window> window     = Window::Create(640, 480, "Physics");
    Ref<Renderer> renderer = Renderer::CreateOpenGLRenderer(window);

    bool running = true;
    window->SetCloseCallback([&](Ref<Window> window) {
        running = false;
    });

    Transform cameraTransform  = {};
    cameraTransform.Position.y = 2.0f;
    cameraTransform.Position.z = -5.0f;

    glm::mat4 projectionMatrix =
        glm::perspective(glm::radians(60.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.001f, 1000.0f);

    window->SetResizeCallback([&](Ref<Window> window, uint32_t width, uint32_t height) {
        renderer->OnResize(width, height);
        projectionMatrix =
            glm::perspective(glm::radians(60.0f), (float)window->GetWidth() / (float)window->GetHeight(), 0.001f, 1000.0f);
    });

    // TODO: Should the window keep track of this?
    bool keys[KeyCode_MaxCount] = {};
    window->SetKeyCallback([&](Ref<Window> window, KeyCode key, bool pressed) {
        keys[key] = pressed;

        if (key == KeyCode_Escape && pressed) {
            if (window->IsCursorEnabled()) {
                window->DisableCursor();
            } else {
                window->EnableCursor();
            }
        }
    });

    glm::ivec2 mouseMovement = {};
    window->SetRawMouseMovementCallback([&](Ref<Window> window, int32_t deltaX, int32_t deltaY) {
        mouseMovement.x += deltaX;
        mouseMovement.y += deltaY;
    });

    Ref<Shader> shader = renderer->CreateShader("Basic.shader");

    Mesh crosshairMesh = [&]() -> Mesh {
        glm::vec2 vertices[] = {
            { -1.0f, +1.0f },
            { +1.0f, +1.0f },
            { +1.0f, -1.0f },
            { -1.0f, -1.0f },
        };

        uint32_t indices[] = {
            0, 1, 2, 0, 2, 3,
        };

        VertexBuffer::Element layout[] = {
            { VertexBuffer::ElementType::Float2, 0, sizeof(glm::vec2), false },
        };

        return {
            renderer->CreateVertexBuffer(vertices, sizeof(vertices), layout),
            renderer->CreateIndexBuffer(indices),
        };
    }();

    Mesh cubeMesh     = ObjToMesh(renderer, Obj::Load("Cube.obj")[0]);
    Mesh cylinderMesh = ObjToMesh(renderer, Obj::Load("Cylinder.obj")[0]);
    Mesh sphereMesh   = ObjToMesh(renderer, Obj::Load("Sphere.obj")[0]);

    std::vector<Point*> points;
    std::vector<Stick> sticks;
    std::vector<Cube> cubes;

    cubes.push_back({
        Transform{
            .Position = { -1.5f, 0.0f, 0.0f },
            .Rotation = glm::identity<glm::quat>(),
            .Scale    = { 50.0f, 1.0f, 50.0f },
        },
        glm::vec4{ 0.2f, 0.8f, 0.4f, 1.0f },
    });

    Point* a = points.emplace_back(new Point{
        glm::vec3{ -2.0f, 1.1f, 0.0f },
        glm::vec3{ -2.0f, 1.0f, 0.0f },
        0.5f,
        glm::vec4{ 0.9f, 0.1f, 0.1f, 1.0f },
    });
    Point* b = points.emplace_back(new Point{
        glm::vec3{ 1.95f, 2.2f, 0.0f },
        glm::vec3{ 2.0f, 2.0f, 0.0f },
        0.5f,
        glm::vec4{ 0.9f, 0.1f, 0.1f, 1.0f },
    });

    sticks.push_back({
        a,
        b,
        glm::length(b->Position - a->Position),
        glm::vec4{ 0.6f, 0.1f, 0.0f, 1.0f },
    });

    Clock clock;
    clock.Start();
    double lastTime = clock.GetElapsed();
    float fixedTime = 0.0f;

    window->Show();
    window->DisableCursor();
    while (running) {
        mouseMovement = {};
        window->Update();

        double time = clock.GetElapsed();
        float dt    = (float)(time - lastTime);
        lastTime    = time;

        // Update
        {
            // Camera
            {
                float speed       = keys[KeyCode_Control] ? 5.0f : 2.0f;
                glm::vec3 forward = cameraTransform.Rotation * glm::vec3{ 0.0f, 0.0f, 1.0f };
                glm::vec3 right   = cameraTransform.Rotation * glm::vec3{ 1.0f, 0.0f, 0.0f };
                glm::vec3 up      = cameraTransform.Rotation * glm::vec3{ 0.0f, 1.0f, 0.0f };

                float cameraSensitivity  = 1.0f;
                glm::vec2 cameraMovement = (glm::vec2)mouseMovement /
                                           glm::vec2((float)window->GetWidth(), (float)window->GetHeight()) * cameraSensitivity;

                cameraTransform.Rotation =
                    glm::rotate(glm::identity<glm::quat>(), cameraMovement.x, up) * cameraTransform.Rotation;
                cameraTransform.Rotation =
                    glm::rotate(glm::identity<glm::quat>(), cameraMovement.y, right) * cameraTransform.Rotation;

                float cameraRotationSpeed = 90.0f;
                float cameraRotation =
                    ((keys[KeyCode_Q] ? cameraRotationSpeed : 0) + (keys[KeyCode_E] ? -cameraRotationSpeed : 0)) * dt;
                cameraTransform.Rotation =
                    glm::rotate(glm::identity<glm::quat>(), glm::radians(cameraRotation), forward) * cameraTransform.Rotation;

                if (keys[KeyCode_W])
                    cameraTransform.Position += forward * speed * dt;
                if (keys[KeyCode_S])
                    cameraTransform.Position -= forward * speed * dt;
                if (keys[KeyCode_A])
                    cameraTransform.Position -= right * speed * dt;
                if (keys[KeyCode_D])
                    cameraTransform.Position += right * speed * dt;
                if (keys[KeyCode_Space])
                    cameraTransform.Position += up * speed * dt;
                if (keys[KeyCode_Shift])
                    cameraTransform.Position -= up * speed * dt;
            }

            glm::vec3 cameraForward = cameraTransform.Rotation * glm::vec3{ 0.0f, 0.0f, 1.0f };

            fixedTime += dt;
            constexpr float FixedInterval = 1.0 / 60.0f;
            while (fixedTime >= FixedInterval) {
                for (auto& point : points) {
                    glm::vec3 velocity      = point->Position - point->PreviousPosition;
                    point->PreviousPosition = point->Position;

                    point->Position += velocity;
                    point->Position.y -= 0.002f;

                    if (point->Position.y < 0.5f + point->Radius) {
                        point->Position.y          = 0.5f + point->Radius;
                        point->PreviousPosition.y  = point->Position.y + velocity.y * 0.5f;
                        point->PreviousPosition.xy = point->Position.xy + velocity.xy * 0.2f;
                    }
                }
                for (auto& stick : sticks) {
                    float distance   = glm::distance(stick.A->Position, stick.B->Position);
                    float difference = distance - stick.Length;
                    glm::vec3 aToB   = glm::normalize(stick.B->Position - stick.A->Position);
                    stick.A->Position += aToB * difference * 0.5f * 0.1f;
                    stick.B->Position -= aToB * difference * 0.5f * 0.1f;
                }
                fixedTime -= FixedInterval;
            }
        }

        // Render
        {
            renderer->Clear({ 0.2f, 0.4f, 0.8f, 1.0f });
            // Game
            renderer->BeginScene(cameraTransform, projectionMatrix, true);
            for (auto& cube : cubes) {
                renderer->DrawIndexed(cubeMesh.VertexBuffer, cubeMesh.IndexBuffer, shader, cube.Transform, cube.Color);
            }
            for (auto& point : points) {
                Transform transform;
                transform.Position  = point->Position;
                transform.Scale.xyz = point->Radius * 2.0f;
                renderer->DrawIndexed(sphereMesh.VertexBuffer, sphereMesh.IndexBuffer, shader, transform, point->Color);
            }
            for (auto& stick : sticks) {
                Transform transform;
                transform.Position = stick.A->Position;
                transform.Rotation =
                    glm::quatLookAt(glm::normalize(stick.B->Position - stick.A->Position), glm::vec3{ 0.0f, 1.0f, 0.0f });
                transform.Scale.xy = 0.5f;
                transform.Scale.z  = glm::length(stick.B->Position - stick.A->Position);
                renderer->DrawIndexed(cylinderMesh.VertexBuffer, cylinderMesh.IndexBuffer, shader, transform, stick.Color);
            }
            renderer->EndScene();

            // UI
            renderer->BeginScene(
                glm::identity<glm::mat4>(),
                glm::scale(glm::identity<glm::mat4>(), { 1.0f, (float)window->GetWidth() / (float)window->GetHeight(), 1.0f }),
                false);
            renderer->DrawIndexed(crosshairMesh.VertexBuffer,
                                  crosshairMesh.IndexBuffer,
                                  shader,
                                  glm::scale(glm::identity<glm::mat4>(), { 0.005f, 0.005f, 1.0f }),
                                  { 0.1f, 0.1f, 0.1f, 1.0f });
            renderer->EndScene();
        }

        renderer->Present();
    }
    window->EnableCursor();
    window->Hide();

    return 0;
}
