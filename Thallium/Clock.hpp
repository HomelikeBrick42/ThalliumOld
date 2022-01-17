#pragma once

#include "Base.hpp"

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
