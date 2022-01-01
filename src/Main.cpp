#include "Base.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "OpenGLRenderer.hpp"
#include "OpenGLShader.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

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

    Ref<OpenGLShader> shader = renderer->CreateShader("Basic.shader").As<OpenGLShader>();

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

        renderer->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        renderer->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Matrix4x4f projectionMatrix = Matrix4x4f_Identity();
        projectionMatrix[3][0]      = 0.5f;
        projectionMatrix[3][1]      = -0.5f;
        projectionMatrix[3][2]      = 0.0f;

        shader->Bind();
        renderer->glUniformMatrix4fv(2, 1, false, &projectionMatrix[0][0]);
        renderer->DrawIndexed(vertexBuffer, indexBuffer, shader);

        renderer->Present();
    }
    window->Hide();

    return 0;
}
