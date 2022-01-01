#include "Window.hpp"
#include "Renderer.hpp"
#include "Clock.hpp"

#include "Transform.hpp"

#include <iostream>
#include <format>

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

    Ref<Shader> shader = renderer->CreateShader("Basic.shader");

    struct Vertex {
        glm::vec3 Position;
    };

    Vertex vertices[] = {
        { .Position = { -0.5, +0.5, 0.0 } },
        { .Position = { +0.5, +0.5, 0.0 } },
        { .Position = { +0.5, -0.5, 0.0 } },
        { .Position = { -0.5, -0.5, 0.0 } },
    };

    VertexBuffer::Element layout[] = {
        { VertexBuffer::ElementType::Float3, offsetof(Vertex, Position), sizeof(Vertex), false },
    };

    Ref<VertexBuffer> vertexBuffer = renderer->CreateVertexBuffer(vertices, sizeof(vertices), layout);

    uint32_t indices[] = {
        0, 1, 2, 0, 2, 3,
    };

    Ref<IndexBuffer> indexBuffer = renderer->CreateIndexBuffer(indices);

    Clock clock;
    clock.Start();
    double lastTime = clock.GetElapsed();

    window->Show();
    window->DisableCursor();
    while (running) {
        window->Update();

        double time = clock.GetElapsed();
        float dt    = (float)(time - lastTime);
        lastTime    = time;

        float speed       = keys[KeyCode_Control] ? 5.0f : 2.0f;
        glm::vec3 forward = cameraTransform.Rotation * glm::vec3{ 0.0f, 0.0f, 1.0f };
        glm::vec3 right   = cameraTransform.Rotation * glm::vec3{ 1.0f, 0.0f, 0.0f };
        glm::vec3 up      = cameraTransform.Rotation * glm::vec3{ 0.0f, 1.0f, 0.0f };

        if (keys[KeyCode_W])
            cameraTransform.Position += forward * speed * dt;
        if (keys[KeyCode_S])
            cameraTransform.Position -= forward * speed * dt;
        if (keys[KeyCode_A])
            cameraTransform.Position -= right * speed * dt;
        if (keys[KeyCode_D])
            cameraTransform.Position += right * speed * dt;
        if (keys[KeyCode_Q])
            cameraTransform.Position -= up * speed * dt;
        if (keys[KeyCode_E])
            cameraTransform.Position += up * speed * dt;

        // triangleTransform.Rotation = glm::rotate(triangleTransform.Rotation, glm::radians(90.0f) * dt, { 0.0, 0.0, -1.0 });

        renderer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
        renderer->BeginScene(cameraTransform, projectionMatrix);
        renderer->DrawIndexed(vertexBuffer, indexBuffer, shader, triangleTransform);
        renderer->EndScene();

        renderer->Present();
    }
    window->EnableCursor();
    window->Hide();

    return 0;
}
