#include "Window.hpp"
#include "Renderer.hpp"
#include "Clock.hpp"

#include "ObjLoader.hpp"

#include "Transform.hpp"

#include <cmath>

#include <iostream>
#include <format>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

struct Mesh {
    Ref<VertexBuffer> VertexBuffer;
    Ref<IndexBuffer> IndexBuffer;
};

static VertexBuffer::Element VertexLayout[] = {
    { VertexBuffer::ElementType::Float3, offsetof(Vertex, Position), sizeof(Vertex), false },
    { VertexBuffer::ElementType::Float3, offsetof(Vertex, Normal), sizeof(Vertex), false },
};

std::vector<Vertex> ObjMeshToVertices(const Obj::Object& object) {
    std::vector<Vertex> vertices;
    for (auto& face : object.Faces) {
        vertices.push_back({ .Position = object.Positions[face.Positions[0]], .Normal = object.Normals[face.Normals[0]] });
        vertices.push_back({ .Position = object.Positions[face.Positions[1]], .Normal = object.Normals[face.Normals[1]] });
        vertices.push_back({ .Position = object.Positions[face.Positions[2]], .Normal = object.Normals[face.Normals[2]] });
    }
    return vertices;
}

int main(int, char**) {
    Ref<Window> window     = Window::Create(640, 480, "Physics");
    Ref<Renderer> renderer = Renderer::CreateOpenGLRenderer(window);

    bool running = true;
    window->SetCloseCallback([&](Ref<Window> window) {
        running = false;
    });

    Transform cameraTransform  = {};
    cameraTransform.Position.z = -2.0f;

    Transform triangleTransform = {};

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

    std::vector<Obj::Object> objects = Obj::Load("Cube.obj");
    std::vector<Vertex> vertices     = ObjMeshToVertices(objects[0]);
    Ref<VertexBuffer> vertexBuffer =
        renderer->CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), VertexLayout);

    Clock clock;
    clock.Start();
    double lastTime = clock.GetElapsed();

    window->Show();
    window->DisableCursor();
    while (running) {
        mouseMovement = {};
        window->Update();

        double time = clock.GetElapsed();
        float dt    = (float)(time - lastTime);
        lastTime    = time;

        float speed       = keys[KeyCode_Control] ? 5.0f : 2.0f;
        glm::vec3 forward = cameraTransform.Rotation * glm::vec3{ 0.0f, 0.0f, 1.0f };
        glm::vec3 right   = cameraTransform.Rotation * glm::vec3{ 1.0f, 0.0f, 0.0f };
        glm::vec3 up      = cameraTransform.Rotation * glm::vec3{ 0.0f, 1.0f, 0.0f };

        float cameraSensitivity = 1.0f;
        glm::vec2 cameraMovement =
            (glm::vec2)mouseMovement / glm::vec2((float)window->GetWidth(), (float)window->GetHeight()) * cameraSensitivity;

        cameraTransform.Rotation = glm::rotate(glm::identity<glm::quat>(), cameraMovement.x, up) * cameraTransform.Rotation;
        cameraTransform.Rotation = glm::rotate(glm::identity<glm::quat>(), cameraMovement.y, right) * cameraTransform.Rotation;

        float cameraRotationSpeed = 90.0f;
        float cameraRotation = ((keys[KeyCode_Q] ? cameraRotationSpeed : 0) + (keys[KeyCode_E] ? -cameraRotationSpeed : 0)) * dt;
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

        renderer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
        renderer->BeginScene(cameraTransform, projectionMatrix, true);
        renderer->Draw(vertexBuffer, shader, 0, static_cast<uint32_t>(vertices.size()), triangleTransform);
        renderer->EndScene();

        renderer->Present();
    }
    window->EnableCursor();
    window->Hide();

    return 0;
}
