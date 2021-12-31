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
public:
    void SetCloseCallback(CloseCallbackFunc callback) final {
        CloseCallback = callback;
    }
    void SetResizeCallback(ResizeCallbackFunc callback) final {
        ResizeCallback = callback;
    }
private:
    static LRESULT WINAPI StaticWindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT WINAPI WindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
    HINSTANCE Instance = nullptr;
    HWND WindowHandle  = nullptr;
    HDC DeviceContext  = nullptr;
private:
    CloseCallbackFunc CloseCallback   = nullptr;
    ResizeCallbackFunc ResizeCallback = nullptr;
};

#endif
