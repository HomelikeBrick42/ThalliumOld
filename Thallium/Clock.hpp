#pragma once

#include "Base.hpp"

class Clock {
public:
    Clock();
public:
    void Start();
    double GetElapsed();
private:
    double StartTime;
};
