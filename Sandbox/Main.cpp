#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLShader.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLTexture.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Thallium;

int main(int, char**) {
    Ref<Window> window           = Window::Create(640, 480, "Sandbox");
    Ref<OpenGLRenderer> renderer = Renderer::CreateOpenGLRenderer(window).As<OpenGLRenderer>();

    bool running = true;
    window->SetCloseCallback([&](Ref<Window>) {
        running = false;
    });

    window->SetResizeCallback([&](Ref<Window>, uint32_t width, uint32_t height) {
        renderer->OnResize(width, height);
    });

    struct QuadVertex {
        glm::vec2 Position;
        glm::vec2 UV;
    } quadVertices[] = {
        { .Position = { -1.0f, +1.0f }, .UV = { 0.0f, 1.0f } },
        { .Position = { +1.0f, +1.0f }, .UV = { 1.0f, 1.0f } },
        { .Position = { +1.0f, -1.0f }, .UV = { 1.0f, 0.0f } },
        { .Position = { -1.0f, -1.0f }, .UV = { 0.0f, 0.0f } },
    };

    VertexBuffer::Element quadVertexLayout[] = {
        {
            .Type       = VertexBuffer::ElementType::Float2,
            .Offset     = offsetof(QuadVertex, Position),
            .Stride     = sizeof(QuadVertex),
            .Normalized = false,
        },
        {
            .Type       = VertexBuffer::ElementType::Float2,
            .Offset     = offsetof(QuadVertex, UV),
            .Stride     = sizeof(QuadVertex),
            .Normalized = false,
        },
    };

    Ref<VertexBuffer> quadVertexBuffer = renderer->CreateVertexBuffer(quadVertices, sizeof(quadVertices), quadVertexLayout);

    uint32_t quadIndices[]           = { 0, 1, 2, 2, 3, 0 };
    Ref<IndexBuffer> quadIndexBuffer = renderer->CreateIndexBuffer(quadIndices);

    Ref<OpenGLShader> quadShader = renderer->CreateShader("Color.shader").As<OpenGLShader>();

    Material quadMaterial = {
        .Color = { 1.0f, 1.0f, 1.0f, 1.0f },
    };

    glm::u8vec4 textureData[] = {
        { 0xFF, 0xFF, 0x00, 0xFF },
    };

    Ref<OpenGLTexture> texture = renderer->CreateTexture(textureData, 1, 1).As<OpenGLTexture>();

    texture->Bind(0);
    quadShader->SetIntUniform("u_Texture", 0);

    window->Show();
    while (running) {
        window->Update();
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->BeginScene({}, glm::identity<glm::mat4>(), false);
        renderer->DrawIndexed(quadVertexBuffer, quadIndexBuffer, quadShader, {}, quadMaterial);
        renderer->EndScene();
        renderer->Present();
    }
    window->Hide();

    return 0;
}
