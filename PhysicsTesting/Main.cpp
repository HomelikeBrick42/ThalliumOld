#include "Thallium/Core/Base.hpp"

#include "Thallium/Core/Window.hpp"
#include "Thallium/Core/Keycode.hpp"
#include "Thallium/Core/Clock.hpp"

#include "Thallium/Renderer/Renderer.hpp"

#include "Thallium/Scene/Scene.hpp"
#include "Thallium/Scene/Entity.hpp"
#include "Thallium/Scene/Transform.hpp"

#include <glm/glm.hpp>

using namespace Thallium;

struct Circle {
    glm::vec2 Velocity;
};

void UpdatePhysics(Ref<Scene> scene, float dt) {
    std::vector<std::pair<Transform&, Circle&>> Circles;
    scene->IterateComponents(
        std::function<void(EntityID, Transform&, Circle&)>([&](EntityID id, Transform& transform, Circle& circle) {
            Circles.emplace_back(transform, circle);

            transform.Position.xy += circle.Velocity * dt;
        }));

    for (size_t a = 0; a < Circles.size(); a++) {
        auto [transform_a, circle_a] = Circles[a];
        for (size_t b = a + 1; b < Circles.size(); b++) {
            auto [transform_b, circle_b] = Circles[b];
            if (glm::distance(transform_a.Position, transform_b.Position) <=
                (glm::compMul(transform_a.Scale) + glm::compMul(transform_b.Scale)) * 0.5f) {
                glm::vec2 aToB    = glm::normalize(transform_b.Position - transform_a.Position);
                circle_a.Velocity = glm::reflect(circle_a.Velocity, aToB);
                circle_b.Velocity = glm::reflect(circle_b.Velocity, -aToB);
            }
        }
    }
}

int main(int, char**) {
    Ref<Scene> scene = Scene::Create();

    Entity a(scene);
    a.AddComponent<Transform>({ .Position = { 2.0f, 0.0f, 0.0f } });
    a.AddComponent<Circle>({ .Velocity = { -1.5f, 1.0f } });

    Entity b(scene);
    b.AddComponent<Transform>({ .Position = { -2.0f, 0.0f, 0.0f } });
    b.AddComponent<Circle>({ .Velocity = { 0.5f, 0.75f } });

    Ref<Window> window = Window::Create(640, 480, "2D Physics");

    bool running = true;
    window->SetCloseCallback([&](Ref<Window>) {
        running = false;
    });

    Transform cameraTransform;
    float cameraZoom = 1.0f;

    auto calculateProjectionMatrix = [&]() -> glm::mat4 {
        return glm::ortho(-(float)window->GetWidth() / (float)window->GetHeight() * cameraZoom,
                          (float)window->GetWidth() / (float)window->GetHeight() * cameraZoom,
                          -cameraZoom,
                          cameraZoom,
                          -1.0f,
                          1.0f);
    };

    glm::mat4 cameraProjectionMatrix = calculateProjectionMatrix();

    window->SetResizeCallback([&](Ref<Window>, uint32_t width, uint32_t height) {
        cameraProjectionMatrix = calculateProjectionMatrix();
    });

    window->SetScrollCallback([&](Ref<Window>, int8_t direction) {
        if (direction > 0) {
            cameraZoom -= cameraZoom * 0.1f;
        } else if (direction < 0) {
            cameraZoom += cameraZoom * 0.1f;
        }
        cameraProjectionMatrix = calculateProjectionMatrix();
    });

    bool keys[KeyCode_MaxCount] = {};
    window->SetKeyCallback([&](Ref<Window>, KeyCode key, bool pressed) {
        keys[key] = pressed;
    });

    Ref<Renderer> renderer = Renderer::CreateOpenGLRenderer(window);

    Ref<Shader> circleShader = renderer->CreateShader("Circle.shader");

    struct CircleVertex {
        glm::vec2 Position;
        glm::vec2 Coord;
    };

    VertexBuffer::Element circleVertexLayout[] = {
        { Thallium::VertexBuffer::ElementType::Float2, offsetof(CircleVertex, Position), sizeof(CircleVertex), false },
        { Thallium::VertexBuffer::ElementType::Float2, offsetof(CircleVertex, Coord), sizeof(CircleVertex), false },
    };

    CircleVertex circleVertices[] = {
        { .Position = { -0.5f, +0.5f }, .Coord = { -1.0f, +1.0f } },
        { .Position = { +0.5f, +0.5f }, .Coord = { +1.0f, +1.0f } },
        { .Position = { +0.5f, -0.5f }, .Coord = { +1.0f, -1.0f } },
        { .Position = { -0.5f, -0.5f }, .Coord = { -1.0f, -1.0f } },
    };

    Ref<VertexBuffer> circleVertexBuffer =
        renderer->CreateVertexBuffer(circleVertices, sizeof(circleVertices), circleVertexLayout);

    uint32_t circleIndices[] = {
        0, 1, 2, 2, 3, 0,
    };

    Ref<IndexBuffer> circleIndexBuffer = renderer->CreateIndexBuffer(circleIndices);

    Clock clock;
    clock.Start();
    double lastTime = clock.GetElapsed();
    float fixedTime = 0.0f;

    window->Show();
    while (running) {
        window->Update();

        double time = clock.GetElapsed();
        float dt    = (float)(time - lastTime);
        lastTime    = time;

        // Update
        {
            float CameraSpeed = 3.0f * cameraZoom;

            if (keys[KeyCode_W])
                cameraTransform.Position += cameraTransform.GetUp() * CameraSpeed * dt;
            if (keys[KeyCode_S])
                cameraTransform.Position += -cameraTransform.GetUp() * CameraSpeed * dt;
            if (keys[KeyCode_A])
                cameraTransform.Position += -cameraTransform.GetRight() * CameraSpeed * dt;
            if (keys[KeyCode_D])
                cameraTransform.Position += cameraTransform.GetRight() * CameraSpeed * dt;
        }

        // Fixed Update
        {
            fixedTime += dt;
            constexpr float FixedInterval = 1.0f / 50.0f;
            while (fixedTime >= FixedInterval) {
                UpdatePhysics(scene, FixedInterval);
                fixedTime -= FixedInterval;
            }
        }

        // Render
        {
            renderer->BeginScene(cameraTransform, cameraProjectionMatrix, false);

            renderer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

            scene->IterateComponents(
                std::function<void(EntityID, Transform&, Circle&)>([&](EntityID id, Transform& transform, Circle& circle) {
                    renderer->DrawIndexed(circleVertexBuffer,
                                          circleIndexBuffer,
                                          circleShader,
                                          transform,
                                          {
                                              .Color = { 1.0f, 0.0f, 0.0f, 1.0f },
                                          });
                }));

            renderer->EndScene();
        }

        renderer->Present();
    }
    window->Hide();

    return 0;
}
