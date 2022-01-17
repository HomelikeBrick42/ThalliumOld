#pragma once

#include "Thallium/Core/Base.hpp"

namespace Thallium {

    class Clock {
    public:
        Clock();
    public:
        void Start();
        double GetElapsed();
    private:
        double StartTime;
    };

}
