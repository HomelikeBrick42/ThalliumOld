#include "Thallium/Core/Windows/WindowsWindow.hpp"

#include <iostream>
#include <format>

#if defined(_WIN32) || defined(_WIN64)

namespace Thallium {

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

        Width  = width;
        Height = height;

        RECT windowRect;
        windowRect.left   = 100;
        windowRect.right  = windowRect.left + static_cast<LONG>(Width);
        windowRect.top    = 100;
        windowRect.bottom = windowRect.top + static_cast<LONG>(Height);
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

        // TODO: Check if this works with multiple windows
        RAWINPUTDEVICE rawInputDevice = {
            .usUsagePage = 0x01,
            .usUsage     = 0x02,
        };
        if (!RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE))) {
            std::cerr << std::format("Unable to register raw input: 0x{:x}", GetLastError()) << std::endl;
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

    void WindowsWindow::EnableCursor() {
        while (ShowCursor(true) < 0) {
        }
        ClipCursor(nullptr);
        CursorDisabled = false;
    }

    void WindowsWindow::DisableCursor() {
        while (ShowCursor(false) >= 0) {
        }
        RECT rect = {};
        GetClientRect(WindowHandle, &rect);
        MapWindowPoints(WindowHandle, nullptr, reinterpret_cast<POINT*>(&rect), sizeof(RECT) / sizeof(POINT));
        ClipCursor(&rect);
        CursorDisabled = true;
    }

    LRESULT WINAPI WindowsWindow::StaticWindowMessageCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_NCCREATE) {
            CREATESTRUCTA* createStruct = reinterpret_cast<CREATESTRUCTA*>(lParam);
            SetWindowLongPtrA(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        }

        WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrA(hWnd, GWLP_USERDATA));
        if (window)
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
                    CloseCallback(*this);
            } break;

            case WM_SIZE: {
                if (ResizeCallback) {
                    RECT windowRect = {};
                    GetClientRect(hWnd, &windowRect);
                    LONG windowWidth  = windowRect.right - windowRect.left;
                    LONG windowHeight = windowRect.bottom - windowRect.top;
                    if (windowWidth > 0 && windowHeight > 0) {
                        Width  = static_cast<uint32_t>(windowWidth);
                        Height = static_cast<uint32_t>(windowHeight);
                        ResizeCallback(*this, Width, Height);
                    }
                }
            } break;

            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP: {
                if (KeyCallback) {
                    bool pressed = message == WM_KEYDOWN || message == WM_SYSKEYDOWN;
                    KeyCode key  = KeyCode_Unknown;
                    switch (wParam) {
                        case 'W': {
                            key = KeyCode_W;
                        } break;

                        case 'S': {
                            key = KeyCode_S;
                        } break;

                        case 'A': {
                            key = KeyCode_A;
                        } break;

                        case 'D': {
                            key = KeyCode_D;
                        } break;

                        case 'Q': {
                            key = KeyCode_Q;
                        } break;

                        case 'E': {
                            key = KeyCode_E;
                        } break;

                        case VK_SHIFT: {
                            key = KeyCode_Shift;
                        } break;

                        case VK_CONTROL: {
                            key = KeyCode_Control;
                        } break;

                        case VK_SPACE: {
                            key = KeyCode_Space;
                        } break;

                        case VK_ESCAPE: {
                            key = KeyCode_Escape;
                        } break;

                        default: {
                            key = KeyCode_Unknown;
                        } break;
                    }
                    for (size_t i = 0; i < (size_t)(lParam & 0xFFFF); i++) {
                        KeyCallback(*this, key, pressed);
                    }
                }
                result = DefWindowProcA(hWnd, message, wParam, lParam);
            } break;

            case WM_SETFOCUS: {
                if (CursorDisabled) {
                    while (ShowCursor(false) >= 0) {
                    }
                    RECT rect = {};
                    GetClientRect(WindowHandle, &rect);
                    MapWindowPoints(WindowHandle, nullptr, reinterpret_cast<POINT*>(&rect), sizeof(RECT) / sizeof(POINT));
                    ClipCursor(&rect);
                }
            } break;

            case WM_KILLFOCUS: {
                if (CursorDisabled) {
                    while (ShowCursor(true) < 0) {
                    }
                    ClipCursor(nullptr);
                }
            } break;

            case WM_INPUT: {
                if (RawMouseMovementCallback) {
                    HRAWINPUT rawInput = reinterpret_cast<HRAWINPUT>(lParam);
                    UINT size          = 0;
                    if (GetRawInputData(rawInput, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == UINT_MAX) {
                        std::cerr << std::format("Unable to get the size of the mouse input data: {}", GetLastError())
                                  << std::endl;
                        break;
                    }
                    char* bytes = new char[size];
                    defer(delete[] bytes);
                    if (GetRawInputData(rawInput, RID_INPUT, bytes, &size, sizeof(RAWINPUTHEADER)) == UINT_MAX) {
                        std::cerr << std::format("Unable to get the mouse input data: {}", GetLastError()) << std::endl;
                        break;
                    }
                    RAWINPUT* input = reinterpret_cast<RAWINPUT*>(bytes);
                    int32_t mouseX  = static_cast<int32_t>(input->data.mouse.lLastX);
                    int32_t mouseY  = static_cast<int32_t>(input->data.mouse.lLastY);
                    RawMouseMovementCallback(*this, mouseX, mouseY);
                }
            } break;

            case WM_LBUTTONDOWN: {
                if (MouseButtonCallback)
                    MouseButtonCallback(*this, MouseButton_Left, true);
            } break;

            case WM_LBUTTONUP: {
                if (MouseButtonCallback)
                    MouseButtonCallback(*this, MouseButton_Left, false);
            } break;

            case WM_MBUTTONDOWN: {
                if (MouseButtonCallback)
                    MouseButtonCallback(*this, MouseButton_Middle, true);
            } break;

            case WM_MBUTTONUP: {
                if (MouseButtonCallback)
                    MouseButtonCallback(*this, MouseButton_Middle, false);
            } break;

            case WM_RBUTTONDOWN: {
                if (MouseButtonCallback)
                    MouseButtonCallback(*this, MouseButton_Right, true);
            } break;

            case WM_RBUTTONUP: {
                if (MouseButtonCallback)
                    MouseButtonCallback(*this, MouseButton_Right, false);
            } break;

            case WM_MOUSEWHEEL: {
                int8_t direction = 0;
                if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
                    direction = 1;
                } else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                    direction = -1;
                }
                if (ScrollCallback && direction != 0)
                    ScrollCallback(*this, direction);
            } break;

            case WM_MOUSEMOVE: {
                if (MouseMoveCallback)
                    MouseMoveCallback(*this, LOWORD(lParam), HIWORD(lParam));
            } break;

            default: {
                result = DefWindowProcA(hWnd, message, wParam, lParam);
            } break;
        }

        return result;
    }

}

#endif
