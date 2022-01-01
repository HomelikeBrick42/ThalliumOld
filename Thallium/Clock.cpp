#include "Clock.hpp"

#if defined(_WIN32) || defined(_WIN64)

    #include <Windows.h>

static bool Initialized = false;
static LARGE_INTEGER StartInteger;
static double Frequency;

Clock::Clock() {
    if (!Initialized) {
        QueryPerformanceCounter(&StartInteger);
        LARGE_INTEGER frequency = {};
        QueryPerformanceFrequency(&frequency);
        Frequency   = 1.0 / (double)frequency.QuadPart;
        Initialized = true;
    }
    LARGE_INTEGER time = {};
    QueryPerformanceCounter(&time);
    StartTime = (double)(time.QuadPart - StartInteger.QuadPart) * Frequency;
}

void Clock::Start() {
    LARGE_INTEGER time = {};
    QueryPerformanceCounter(&time);
    StartTime = (double)(time.QuadPart - StartInteger.QuadPart) * Frequency;
}

double Clock::GetElapsed() {
    LARGE_INTEGER time = {};
    QueryPerformanceCounter(&time);
    double current = (double)(time.QuadPart - StartInteger.QuadPart) * Frequency;
    return current - StartTime;
}

#endif
