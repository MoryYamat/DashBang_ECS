#pragma once

#include <array>
#include <chrono>

namespace ddknd::clock
{
    // @note Convert seconds to milliseconds internally.
    class FrameTimeStatistics
    {
        public:
            static constexpr std::size_t kMaxSamples = 512;
            static constexpr float kUpdateIntervalSeconds = 0.5f;
            static constexpr float kP90 = 0.90f;
            static constexpr float kP99 = 0.99f;

            void PushFrame(float deltaTimeSeconds);

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
            void Recalculate();

            static float Percentile(std::array<float, kMaxSamples>::const_iterator first, std::size_t count, float percentile);

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
        void Tick();

        float DeltaSeconds() const
        {
            return deltaSeconds_;
        }

        // Average FPS over a 0.5-second interval
        float FPS() const
        {
            return fps_;
        }

      private:
        using Clock = std::chrono::steady_clock;

        bool initialized_ = false;
        Clock::time_point prev_{};

        float deltaSeconds_ = 0.0f;
        float fps_ = 0.0f;

        float fpsAccumSeconds_ = 0.0f;
        int fpsFrameCount_ = 0;
    };
} // namespace ddknd::clock