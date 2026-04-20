#pragma once

#include <cstdint>

namespace ddknd::clock
{
    struct Clock
    {
        float frameDt = 0.0f;
        float timeScale = 1.0f;

        float fixedDt = 1.0f / 60;
        float accum = 0.0f;
        float alpha = 0.0f;
        int stepsThisFrame = 0;
        std::uint32_t frame = 0;
        std::uint32_t tick = 0;
    };
}// namespace ddknd::clock