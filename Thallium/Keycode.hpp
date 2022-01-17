#pragma once

#include "Base.hpp"

namespace Thallium {

    // TODO: Add more keycodes
    enum KeyCode {
        KeyCode_Unknown,

        KeyCode_W,
        KeyCode_S,
        KeyCode_A,
        KeyCode_D,

        KeyCode_Q,
        KeyCode_E,

        KeyCode_Shift,
        KeyCode_Control,
        KeyCode_Space,

        KeyCode_Escape,

        KeyCode_MaxCount,
    };

    enum MouseButton {
        MouseButton_Left,
        MouseButton_Middle,
        MouseButton_Right,
    };

}
