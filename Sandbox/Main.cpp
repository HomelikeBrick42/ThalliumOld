#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"
#include "Thallium/Core/Clock.hpp"

#include "Camera.hpp"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <iostream>

using namespace Thallium;

struct Portal {
    Portal(Transform in, Transform out, Ref<Renderer> renderer, float fov, size_t width, size_t height)
        : In(in)
        , Out(out)
        , MainTexture(renderer->CreateFramebuffer(renderer->CreateTexture(width, height), true))
        , LastTexture(renderer->CreateFramebuffer(renderer->CreateTexture(width, height), true))
        , ProjectionMatrix(glm::perspective(glm::radians(fov), (float)width / (float)height, 0.001f, 1000.0f)) {}

    Transform In;
    Transform Out;
    Ref<Framebuffer> MainTexture;
    Ref<Framebuffer> LastTexture;
    glm::mat4 ProjectionMatrix;
};

int main(int, char**) {
    Ref<Window> window     = Window::Create(640, 480, "Sandbox");
    Ref<Renderer> renderer = Renderer::CreateOpenGLRenderer(window);

    bool running = true;
    window->SetCloseCallback([&](Ref<Window>) {
        running = false;
    });

    Ref<Framebuffer> framebuffer =
        renderer->CreateFramebuffer(renderer->CreateTexture(window->GetWidth(), window->GetHeight()), true);

    Camera camera(3.0f, 1.0f, 60.0f, window->GetWidth(), window->GetHeight());
    camera.Transform.Position.z = -2.0f;

    Portal portal(
        {
            .Position = { -1.0f, 0.0f, 0.0f },
        },
        {
            .Position = { -1.0f, 0.0f, -3.0f },
        },
        renderer,
        camera.FOV,
        1024,
        1024);

    window->SetResizeCallback([&](Ref<Window>, uint32_t width, uint32_t height) {
        framebuffer->Resize(width, height);
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

    Ref<Shader> quadShader   = renderer->CreateShader("Color.shader");
    Ref<Shader> portalShader = renderer->CreateShader("Portal.shader");

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
    window->DisableCursor();
    while (running) {
        window->Update();

        double time = clock.GetElapsed();
        float dt    = (float)(time - lastTime);
        defer(lastTime = time);

        camera.OnUpdate(dt);

        constexpr size_t RecursionLimit = 3;
        glm::mat4 portalMatrices[RecursionLimit];
        glm::mat4 currentPortalMatrix = camera.Transform;
        for (size_t i = 0; i < RecursionLimit; i++) {
            currentPortalMatrix = portal.Out.ToMatrix() * glm::inverse(portal.In.ToMatrix()) * currentPortalMatrix;
            portalMatrices[RecursionLimit - i - 1] = currentPortalMatrix;
        }

        for (size_t i = 0; i < RecursionLimit; i++) {
            // Render scene from the portal's view
            renderer->BeginScene(portalMatrices[i], portal.ProjectionMatrix, true, portal.MainTexture);
            renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
            renderer->DrawIndexed(quadVertexBuffer,
                                  quadIndexBuffer,
                                  quadShader,
                                  {
                                      .Position = { 0.5f, 0.0f, -1.0f },
                                      .Scale    = { 0.5f, 0.5f, 0.5f },
                                  },
                                  quadMaterial);

            renderer->DrawIndexed(quadVertexBuffer,
                                  quadIndexBuffer,
                                  portalShader,
                                  portal.In,
                                  {
                                      .Color   = { 0.9f, 0.9f, 0.9f, 1.0f },
                                      .Texture = portal.LastTexture->GetColorAttachment(),
                                  });
            renderer->EndScene();
            portal.MainTexture->CopyInto(portal.LastTexture);
        }

        // Render scene
        renderer->BeginScene(camera.Transform, camera.ProjectionMatrix, true, framebuffer);
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              quadShader,
                              {
                                  .Position = { 1.0f, 0.0f, -1.0f },
                              },
                              quadMaterial);

        renderer->DrawIndexed(quadVertexBuffer,
                              quadIndexBuffer,
                              portalShader,
                              portal.In,
                              {
                                  .Color   = { 0.9f, 0.9f, 0.9f, 1.0f },
                                  .Texture = portal.MainTexture->GetColorAttachment(),
                              });
        renderer->EndScene();

        // Put framebuffer onto screen
        renderer->BeginScene(glm::identity<glm::mat4>(), glm::identity<glm::mat4>(), false);
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->DrawIndexed(
            quadVertexBuffer, quadIndexBuffer, quadShader, {}, { .Texture = framebuffer->GetColorAttachment() });
        renderer->EndScene();

        renderer->Present();
    }
    window->EnableCursor();
    window->Hide();

    return 0;
}
