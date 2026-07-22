#pragma once

namespace platform {

class Time {
public:
    Time();

    /** Seconds since previous tick (clamped). */
    float tick();

    static double nowSeconds();

private:
    double last_ = 0.0;
    float maxDelta_ = 0.05f;
};

} // namespace platform
