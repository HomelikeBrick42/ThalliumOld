#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Window.hpp"

#if defined(_WIN32) || defined(_WIN64)

    #include <Windows.h>

namespace Thallium {

    class WindowsWindow final: public Window {
        friend class WindowsOpenGLRenderer;
    public:
        WindowsWindow(uint32_t width, uint32_t height, const std::string& title);
        ~WindowsWindow() final;

        void Update() final;

        void Show() final;
        void Hide() final;

        void EnableCursor() final;
        void DisableCursor() final;
        bool IsCursorEnabled() final {
            return !CursorDisabled;
        }

        uint32_t GetWidth() final {
            return Width;
        }
        uint32_t GetHeight() final {
            return Height;
        }
    public:
        void SetCloseCallback(CloseCallbackFunc callback) final {
            CloseCallback = callback;
        }
        void SetResizeCallback(ResizeCallbackFunc callback) final {
            ResizeCallback = callback;
        }
        void SetKeyCallback(KeyCallbackFunc callback) final {
            KeyCallback = callback;
        }
        void SetMouseButtonCallback(MouseButtonCallbackFunc callback) final {
            MouseButtonCallback = callback;
        }
        void SetScrollCallback(ScrollCallbackFunc callback) final {
            ScrollCallback = callback;
        }
        void SetMouseMoveCallback(MouseMoveCallbackFunc callback) final {
            MouseMoveCallback = callback;
        }
        void SetRawMouseMovementCallback(RawMouseMovementCallbackFunc callback) final {
            RawMouseMovementCallback = callback;
        }
    private:
        static LRESULT WINAPI StaticWindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        LRESULT WINAPI WindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    private:
        HINSTANCE Instance = nullptr;
        HWND WindowHandle  = nullptr;
        HDC DeviceContext  = nullptr;
    private:
        uint32_t Width      = 0;
        uint32_t Height     = 0;
        bool CursorDisabled = false;
    private:
        CloseCallbackFunc CloseCallback                       = nullptr;
        ResizeCallbackFunc ResizeCallback                     = nullptr;
        KeyCallbackFunc KeyCallback                           = nullptr;
        MouseButtonCallbackFunc MouseButtonCallback           = nullptr;
        ScrollCallbackFunc ScrollCallback                     = nullptr;
        MouseMoveCallbackFunc MouseMoveCallback               = nullptr;
        RawMouseMovementCallbackFunc RawMouseMovementCallback = nullptr;
    };

}

#endif
