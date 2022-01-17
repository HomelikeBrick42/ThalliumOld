#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <iostream>

using namespace Thallium;

int main(int, char**) {
    Ref<Window> window     = Window::Create(640, 480, "Sandbox");
    Ref<Renderer> renderer = Renderer::CreateOpenGLRenderer(window);

    bool running = true;
    window->SetCloseCallback([&](Ref<Window>) {
        running = false;
    });

    Ref<Framebuffer> framebuffer =
        renderer->CreateFramebuffer(renderer->CreateTexture(window->GetWidth(), window->GetHeight()), true);

    window->SetResizeCallback([&](Ref<Window>, uint32_t width, uint32_t height) {
        renderer->OnResize(width, height);
        framebuffer->GetColorAttachment()->AllocatePixels(width, height);
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

    Ref<Shader> quadShader = renderer->CreateShader("Color.shader");

    Material quadMaterial = {
        .Color = { 1.0f, 1.0f, 1.0f, 1.0f },
    };

    glm::vec4 textureData[] = {
        { 1.0f, 1.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
    };
    quadMaterial.Texture = renderer->CreateTexture(textureData, 2, 2);

    window->Show();
    while (running) {
        window->Update();
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });

        // Render a scene where there is a textured quad covering the screen to the framebuffer
        renderer->BeginScene({}, glm::identity<glm::mat4>(), false, framebuffer);
        renderer->DrawIndexed(quadVertexBuffer, quadIndexBuffer, quadShader, {}, quadMaterial);
        renderer->EndScene();

        // Render the framebuffer onto the actual screen at half size
        renderer->BeginScene({}, glm::identity<glm::mat4>(), false);
        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              quadShader,
                              { .Scale = { 0.5f, 0.5f, 0.5f } },
                              { .Texture = framebuffer->GetColorAttachment() });
        renderer->EndScene();

        renderer->Present();
    }
    window->Hide();

    return 0;
}
