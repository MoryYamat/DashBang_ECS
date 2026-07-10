#pragma once

#include <array>
#include <algorithm>

#include <cstdint>
#include <chrono>

namespace ddknd::clock
{
    // Unused
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

    // Convert to milliseconds internally.
    class FrameTimeStatistics
    {
        public:
            static constexpr std::size_t kMaxSamples = 512;
            static constexpr float kUpdateIntervalSeconds = 0.5f;
            static constexpr float kP90 = 0.90f;
            static constexpr float kP99 = 0.99f;

            void PushFrame(float deltaTimeSeconds)
            {
                // From seconds to milliseconds
                const float frameTimeMs = deltaTimeSeconds * 1000.0f;

                currentFrameTimeMs_ = frameTimeMs;

                // ring buffering
                samples_[writeIndex_] = frameTimeMs;
                writeIndex_ = (writeIndex_ + 1) % kMaxSamples;

                if(sampleCount_ < kMaxSamples)
                {
                    ++sampleCount_;
                }

                elapsedSinceUpdate_ += deltaTimeSeconds;

                if(elapsedSinceUpdate_ >= kUpdateIntervalSeconds)
                {
                    Recalculate();
                    elapsedSinceUpdate_ = 0.0f;
                }
            }

            [[nodiscard]] float CurrentFrameTimeMs() const
            {
                return currentFrameTimeMs_;
            }

            [[nodiscard]] float P90FrameTimeMs() const
            {
                return p90FrameTimeMs_;
            }

            [[nodiscard]] float P99FrameTimeMs() const
            {
                return p99FrameTimeMs_;
            }


        private:
            void Recalculate()
            {
                if(sampleCount_ == 0)
                    return;

                // copy the values from ring buffer to the work array 
                std::copy_n(samples_.begin(), sampleCount_, workSamples_.begin());

                auto first = workSamples_.begin();
                auto last = first + sampleCount_;

                //  sort the work array
                std::sort(first, last);
                p90FrameTimeMs_ = Percentile(first, sampleCount_, kP90);
                p99FrameTimeMs_ = Percentile(first, sampleCount_, kP99);
            }

            static float Percentile(std::array<float, kMaxSamples>::const_iterator first, std::size_t count, float percentile)
            {
                const float position = percentile * static_cast<float>(count - 1);
                const std::size_t index = static_cast<std::size_t>(std::ceil(position));

                // deref
                return *(first + index);
            }

        private:
            std::array<float, kMaxSamples> samples_{};
            std::array<float, kMaxSamples> workSamples_{};

            std::size_t writeIndex_ = 0;
            std::size_t sampleCount_ = 0;

            float elapsedSinceUpdate_ = 0.0f;
            float currentFrameTimeMs_ = 0.0f;
            float p90FrameTimeMs_ = 0.0f;
            float p99FrameTimeMs_ = 0.0f;
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
                deltaTime_ = 0.0f;
                initialized_ = true;
                return;
            }

            deltaTime_ = std::chrono::duration<float>(now - prev_).count();// this is in seconds, not milliseconds

            prev_ = now;

            fpsAccumTime_ += deltaTime_;
            fpsFrameCount_++;

            // Average FPS over a 0.5-second interval
            if (fpsAccumTime_ >= 0.5f)
            {
                fps_ = static_cast<float>(fpsFrameCount_) / fpsAccumTime_;
                fpsAccumTime_ = 0.0f;
                fpsFrameCount_ = 0;
            }
        }

        // this is in seconds, not milliseconds
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