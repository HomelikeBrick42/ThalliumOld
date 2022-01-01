#pragma once

#include "Base.hpp"
#include "Window.hpp"

#if defined(_WIN32) || defined(_WIN64)

    #include <Windows.h>

class WindowsWindow final: public Window {
    friend class WindowsOpenGLRenderer;
public:
    WindowsWindow(uint32_t width, uint32_t height, const std::string& title);
    ~WindowsWindow() final;

    void Update() final;

    void Show() final;
    void Hide() final;

    void EnableCursor()  final;
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
    CloseCallbackFunc CloseCallback   = nullptr;
    ResizeCallbackFunc ResizeCallback = nullptr;
    KeyCallbackFunc KeyCallback       = nullptr;
};

#endif
