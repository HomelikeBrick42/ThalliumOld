#include "WindowsWindow.hpp"

#include <iostream>
#include <format>

#if defined(_WIN32) || defined(_WIN64)

Ref<Window> Window::Create(uint32_t width, uint32_t height, const std::string& title) {
    return Ref<WindowsWindow>::Create(width, height, title);
}

constexpr const char* WindowClassName = "Physics";
static bool WindowClassInitialised    = false;

WindowsWindow::WindowsWindow(uint32_t width, uint32_t height, const std::string& title) {
    Instance = GetModuleHandleA(nullptr);

    if (!WindowClassInitialised) {
        WNDCLASSEXA windowClass = {
            .cbSize        = sizeof(windowClass),
            .style         = CS_OWNDC,
            .lpfnWndProc   = StaticWindowMessageCallback,
            .hInstance     = Instance,
            .hCursor       = LoadCursor(nullptr, IDC_ARROW),
            .lpszClassName = WindowClassName,
        };
        if (RegisterClassExA(&windowClass) == 0) {
            std::cerr << std::format("Unable to create window class: 0x{:x}", GetLastError()) << std::endl;
            std::exit(1);
        }
        WindowClassInitialised = true;
    }

    constexpr DWORD WindowStyleEx = 0;
    constexpr DWORD WindowStyle   = WS_OVERLAPPEDWINDOW;

    RECT windowRect;
    windowRect.left   = 100;
    windowRect.right  = windowRect.left + static_cast<LONG>(width);
    windowRect.top    = 100;
    windowRect.bottom = windowRect.top + static_cast<LONG>(height);
    AdjustWindowRectEx(&windowRect, WindowStyle, false, WindowStyleEx);

    LONG windowWidth  = windowRect.right - windowRect.left;
    LONG windowHeight = windowRect.bottom - windowRect.top;

    WindowHandle = CreateWindowExA(WindowStyleEx,
                                   WindowClassName,
                                   title.c_str(),
                                   WindowStyle,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   windowWidth,
                                   windowHeight,
                                   nullptr,
                                   nullptr,
                                   Instance,
                                   this);
    if (!WindowHandle) {
        std::cerr << std::format("Unable to create window: 0x{:x}", GetLastError()) << std::endl;
        std::exit(1);
    }

    DeviceContext = GetDC(WindowHandle);
    if (!DeviceContext) {
        std::cerr << std::format("Unable to get device context: 0x{:x}", GetLastError()) << std::endl;
        std::exit(1);
    }
}

WindowsWindow::~WindowsWindow() {
    ReleaseDC(WindowHandle, DeviceContext);
    DestroyWindow(WindowHandle);
}

void WindowsWindow::Update() {
    MSG message;
    while (PeekMessageA(&message, WindowHandle, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void WindowsWindow::Show() {
    ShowWindow(WindowHandle, SW_SHOW);
}

void WindowsWindow::Hide() {
    ShowWindow(WindowHandle, SW_HIDE);
}

LRESULT WINAPI WindowsWindow::StaticWindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NCCREATE) {
        CREATESTRUCTA* createStruct = reinterpret_cast<CREATESTRUCTA*>(lParam);
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
    }

    WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));
    if (window && window->GetRefCount() > 0) // TODO: Do we check the ref count here?
        return window->WindowMessageCallback(hWnd, message, wParam, lParam);
    return DefWindowProcA(hWnd, message, wParam, lParam);
}

LRESULT WINAPI WindowsWindow::WindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (message) {
        case WM_QUIT:
        case WM_DESTROY:
        case WM_CLOSE: {
            if (CloseCallback)
                CloseCallback(this);
        } break;

        case WM_SIZE: {
            if (ResizeCallback) {
                RECT windowRect = {};
                GetClientRect(hWnd, &windowRect);
                LONG windowWidth  = windowRect.right - windowRect.left;
                LONG windowHeight = windowRect.bottom - windowRect.top;
                if (windowWidth > 0 && windowHeight > 0) {
                    ResizeCallback(this, static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight));
                }
            }
        } break;

        default: {
            result = DefWindowProcA(hWnd, message, wParam, lParam);
        } break;
    }

    return result;
}

#endif
