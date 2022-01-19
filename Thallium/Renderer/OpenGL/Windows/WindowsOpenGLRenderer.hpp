#pragma once

#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Thallium/Core/Windows/WindowsWindow.hpp"

#if defined(_WIN32) || defined(_WIN64)

namespace Thallium {

    class WindowsOpenGLRenderer final: public OpenGLRenderer {
    public:
        WindowsOpenGLRenderer(Ref<WindowsWindow> window);
        ~WindowsOpenGLRenderer() final;
    public:
        void Present() final;
    public:
        Ref<Window> GetWindow() const {
            return Window;
        }
    protected:
        void MakeContextCurrent() final;
    private:
        Ref<WindowsWindow> Window = nullptr;
        HGLRC OpenGLContext       = nullptr;
        HMODULE OpenGLLibrary     = nullptr;
    };

}

#endif
