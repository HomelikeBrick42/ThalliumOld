#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"

#include <glm/vec4.hpp>

using namespace Thallium;

int main(int, char**) {
    Ref<Window> window     = Window::Create(640, 480, "Sandbox");
    Ref<Renderer> renderer = Renderer::CreateOpenGLRenderer(window);

    bool running       = true;
    window->SetCloseCallback([&](Ref<Window>) {
        running = false;
    });

    window->SetResizeCallback([&](Ref<Window>, uint32_t width, uint32_t height) {
        renderer->OnResize(width, height);
    });

    window->Show();
    while (running) {
        window->Update();
        renderer->Clear({ 0.2f, 0.3f, 0.8f, 1.0f });
        renderer->Present();
    }

    return 0;
}
