#include "Window.hpp"
#include "Renderer.hpp"

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

    Ref<Shader> shader = renderer->CreateShader("Basic.shader");

    struct Vertex {
        struct {
            float x;
            float y;
            float z;
        } Position;
    };

    Vertex vertices[] = {
        { +0.0, +0.5, 0.0 },
        { +0.5, -0.5, 0.0 },
        { -0.5, -0.5, 0.0 },
    };

    VertexBuffer::Element layout[] = {
        { VertexBuffer::ElementType::Float3, offsetof(Vertex, Position), sizeof(Vertex), false },
    };

    Ref<VertexBuffer> vertexBuffer = renderer->CreateVertexBuffer(vertices, sizeof(vertices), layout);

    uint32_t indices[] = {
        0,
        1,
        2,
    };

    Ref<IndexBuffer> indexBuffer = renderer->CreateIndexBuffer(indices);

    window->Show();
    while (running) {
        window->Update();

        triangleTransform.Rotation = glm::rotate(triangleTransform.Rotation, glm::radians(0.05f), { 0.0, 0.0, -1.0 });

        renderer->Clear({ 0.1f, 0.1f, 0.1f, 1.0f });
        renderer->BeginScene(cameraTransform, projectionMatrix);
        renderer->DrawIndexed(vertexBuffer, indexBuffer, shader, triangleTransform);
        renderer->EndScene();

        renderer->Present();
    }
    window->Hide();

    return 0;
}
