#pragma once

#include "Base.hpp"
#include "Ref.hpp"
#include "Keycode.hpp"

#include <string>
#include <functional>

class Window: public IRef {
protected:
    Window() = default;
public:
    using CloseCallbackFunc  = std::function<void(Ref<Window> window)>;
    using ResizeCallbackFunc = std::function<void(Ref<Window> window, uint32_t width, uint32_t height)>;
    using KeyCallbackFunc    = std::function<void(Ref<Window> window, KeyCode key, bool pressed)>;
public:
    Window(const Window&) = delete;
    Window(Window&&)      = delete;
    virtual ~Window()     = default;
public:
    virtual void Update() = 0;

    virtual void Show() = 0;
    virtual void Hide() = 0;

    virtual void EnableCursor()    = 0;
    virtual void DisableCursor()   = 0;
    virtual bool IsCursorEnabled() = 0;

    virtual uint32_t GetWidth()  = 0;
    virtual uint32_t GetHeight() = 0;
public:
    virtual void SetCloseCallback(CloseCallbackFunc callback)   = 0;
    virtual void SetResizeCallback(ResizeCallbackFunc callback) = 0;
    virtual void SetKeyCallback(KeyCallbackFunc callback)       = 0;
public:
    static Ref<Window> Create(uint32_t width, uint32_t height, const std::string& title);
};
