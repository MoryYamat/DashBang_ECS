#pragma once

#include <cstdint>
#include <chrono>

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

    class FrameTimer
    {
      public:
        void Tick()
        {
            const auto now = Clock::now();

            if (!initialized_)
            {
                prev_ = now;
                initialized_ = true;
                return;
            }

            deltaTime_ = std::chrono::duration<float>(now - prev_).count();

            prev_ = now;

            fpsAccumTime_ += deltaTime_;
            fpsFrameCount_++;

            if (fpsAccumTime_ >= 0.5f)
            {
                fps_ = static_cast<float>(fpsFrameCount_) / fpsAccumTime_;
                fpsAccumTime_ = 0.0f;
                fpsFrameCount_ = 0;
            }
        }

        float DeltaTime() const
        {
            return deltaTime_;
        }
        float FPS() const
        {
            return fps_;
        }

      private:
        using Clock = std::chrono::steady_clock;

        bool initialized_ = false;
        Clock::time_point prev_{};

        float deltaTime_ = 0.0f;
        float fps_ = 0.0f;

        float fpsAccumTime_ = 0.0f;
        int fpsFrameCount_ = 0;
    };
} // namespace ddknd::clock