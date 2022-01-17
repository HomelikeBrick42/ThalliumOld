#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"
#include "Thallium/Core/Clock.hpp"

#include "Camera.hpp"

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
    Ref<Framebuffer> portalFramebuffer =
        renderer->CreateFramebuffer(renderer->CreateTexture(window->GetWidth(), window->GetHeight()), true);
    Ref<Framebuffer> lastPortalFramebuffer =
        renderer->CreateFramebuffer(renderer->CreateTexture(window->GetWidth(), window->GetHeight()), true);

    Camera camera(3.0f, 1.0f, 60.0f, window->GetWidth(), window->GetHeight());
    camera.Transform.Position.z = -2.0f;

    window->SetResizeCallback([&](Ref<Window>, uint32_t width, uint32_t height) {
        renderer->OnResize(width, height);
        framebuffer->Resize(width, height);
        portalFramebuffer->Resize(width, height);
        lastPortalFramebuffer->Resize(width, height);
        camera.Width  = width;
        camera.Height = height;
        camera.UpdateProjectionMatrix();
    });

    window->SetKeyCallback([&](Ref<Window>, KeyCode key, bool pressed) {
        camera.OnKey(key, pressed);
    });

    window->SetRawMouseMovementCallback([&](Ref<Window>, int32_t x, int32_t y) {
        camera.OnMouseMove(x, y);
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

    Clock clock;
    clock.Start();
    double lastTime = clock.GetElapsed();

    window->Show();
    while (running) {
        window->Update();

        double time = clock.GetElapsed();
        float dt    = (float)(time - lastTime);
        defer(lastTime = time);

        camera.OnUpdate(dt);

        // Render scene
        renderer->BeginScene(camera.Transform, camera.ProjectionMatrix, true, framebuffer);
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              quadShader,
                              { .Position = { 0.5f, 0.0f, -1.0f }, .Scale = { 0.5f, 0.5f, 0.5f } },
                              quadMaterial);
        renderer->EndScene();

        // Render scene from the portal's view
        renderer->BeginScene(camera.Transform, camera.ProjectionMatrix, true, portalFramebuffer);
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              quadShader,
                              { .Position = { 0.5f, 0.0f, -1.0f }, .Scale = { 0.5f, 0.5f, 0.5f } },
                              quadMaterial);

        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              quadShader,
                              {
                                  .Position = { -1.0f, 0.0f, 0.0f },
                                  .Scale    = { (float)camera.Width / (float)camera.Height, 1.0f, 1.0f },
                              },
                              {
                                  .Color   = { 0.9f, 0.9f, 0.9f, 1.0f },
                                  .Texture = lastPortalFramebuffer->GetColorAttachment(),
                              });
        renderer->EndScene();

        portalFramebuffer->CopyInto(lastPortalFramebuffer);

        // Render portal
        renderer->BeginScene(camera.Transform, camera.ProjectionMatrix, true, framebuffer);
        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              quadShader,
                              {
                                  .Position = { -1.0f, 0.0f, 0.0f },
                                  .Scale    = { (float)camera.Width / (float)camera.Height, 1.0f, 1.0f },
                              },
                              {
                                  .Color   = { 0.9f, 0.9f, 0.9f, 1.0f },
                                  .Texture = portalFramebuffer->GetColorAttachment(),
                              });
        renderer->EndScene();

        // Put framebuffer onto screen
        renderer->BeginScene({}, glm::identity<glm::mat4>(), false);
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->DrawIndexed(
            quadVertexBuffer, quadIndexBuffer, quadShader, {}, { .Texture = framebuffer->GetColorAttachment() });
        renderer->EndScene();

        renderer->Present();
    }
    window->Hide();

    return 0;
}
