#include "Window.hpp"
#include "Renderer.hpp"
#include "Clock.hpp"

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

Mesh GenerateIcosphere(Ref<Renderer> renderer, uint32_t subdivisions) {
    constexpr float X = 0.525731112119133606f;
    constexpr float Z = 0.850650808352039932f;
    constexpr float N = 0.0f;

    std::vector<Vertex> vertices = {
        { .Position = glm::normalize(glm::vec3{ -X, N, Z }), .Normal = glm::normalize(glm::vec3{ -X, N, Z }) },
        { .Position = glm::normalize(glm::vec3{ X, N, Z }), .Normal = glm::normalize(glm::vec3{ X, N, Z }) },
        { .Position = glm::normalize(glm::vec3{ -X, N, -Z }), .Normal = glm::normalize(glm::vec3{ -X, N, -Z }) },
        { .Position = glm::normalize(glm::vec3{ X, N, -Z }), .Normal = glm::normalize(glm::vec3{ X, N, -Z }) },
        { .Position = glm::normalize(glm::vec3{ N, Z, X }), .Normal = glm::normalize(glm::vec3{ N, Z, X }) },
        { .Position = glm::normalize(glm::vec3{ N, Z, -X }), .Normal = glm::normalize(glm::vec3{ N, Z, -X }) },
        { .Position = glm::normalize(glm::vec3{ N, -Z, X }), .Normal = glm::normalize(glm::vec3{ N, -Z, X }) },
        { .Position = glm::normalize(glm::vec3{ N, -Z, -X }), .Normal = glm::normalize(glm::vec3{ N, -Z, -X }) },
        { .Position = glm::normalize(glm::vec3{ Z, X, N }), .Normal = glm::normalize(glm::vec3{ Z, X, N }) },
        { .Position = glm::normalize(glm::vec3{ -Z, X, N }), .Normal = glm::normalize(glm::vec3{ -Z, X, N }) },
        { .Position = glm::normalize(glm::vec3{ Z, -X, N }), .Normal = glm::normalize(glm::vec3{ Z, -X, N }) },
        { .Position = glm::normalize(glm::vec3{ -Z, -X, N }), .Normal = glm::normalize(glm::vec3{ -Z, -X, N }) },
    };

    std::vector<glm::u32vec3> triangles = {
        { 0, 4, 1 }, { 0, 9, 4 },  { 9, 5, 4 },  { 4, 5, 8 },  { 4, 8, 1 },  { 8, 10, 1 }, { 8, 3, 10 },
        { 5, 3, 8 }, { 5, 2, 3 },  { 2, 7, 3 },  { 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 },
        { 0, 1, 6 }, { 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 },  { 7, 2, 11 },
    };

    return { renderer->CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex), VertexLayout),
             renderer->CreateIndexBuffer({ (uint32_t*)triangles.data(), triangles.size() * 3 }) };
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

    Mesh sphereMesh = GenerateIcosphere(renderer, 1);

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
        renderer->DrawIndexed(sphereMesh.VertexBuffer, sphereMesh.IndexBuffer, shader, triangleTransform);
        renderer->EndScene();

        renderer->Present();
    }
    window->EnableCursor();
    window->Hide();

    return 0;
}
