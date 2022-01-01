#pragma once

#include "OpenGLRenderer.hpp"
#include "WindowsWindow.hpp"

#if defined(_WIN32) || defined(_WIN64)

class WindowsOpenGLRenderer final: public OpenGLRenderer {
public:
    WindowsOpenGLRenderer(Ref<WindowsWindow> window);
    ~WindowsOpenGLRenderer() final;
public:
    void OnResize(uint32_t width, uint32_t height) final;
    void Present() final;
public:
    Ref<Window> GetWindow() {
        return Window;
    }
protected:
    void MakeContextCurrent() final;
private:
    Ref<WindowsWindow> Window = nullptr;
    HGLRC OpenGLContext       = nullptr;
    HMODULE OpenGLLibrary     = nullptr;
};

#endif
