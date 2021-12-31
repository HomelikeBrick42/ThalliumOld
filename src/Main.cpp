#include "Base.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "OpenGLRenderer.hpp"

#include <iostream>
#include <format>

int main(int, char**) {
    Ref<Window> window           = Window::Create(640, 480, "Physics");
    Ref<OpenGLRenderer> renderer = Renderer::CreateOpenGLRenderer(window);

    bool running = true;
    window->SetCloseCallback([&](Ref<Window> window) {
        running = false;
    });
    window->SetResizeCallback([&](Ref<Window> window, uint32_t width, uint32_t height) {
        renderer->OnResize(width, height);
    });

    Ref<Shader> shader = renderer->CreateShader("Basic.shader");

    struct Vector3 {
        float x;
        float y;
        float z;
    };

    struct Vertex {
        Vector3 Position;
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

    window->Show();
    while (running) {
        window->Update();

        renderer->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        renderer->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->Draw(vertexBuffer, shader, 0, 3);

        renderer->Present();
    }
    window->Hide();

    return 0;
}
