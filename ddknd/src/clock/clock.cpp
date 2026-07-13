#include "ddknd/clock/clock.h"

#include <algorithm>
#include <cstdint>


namespace ddknd::clock
{
    void FrameTimeStatistics::PushFrame(float deltaTimeSeconds)
    {
        // From seconds to milliseconds
        const float frameTimeMs = deltaTimeSeconds * 1000.0f;

        currentFrameTimeMs_ = frameTimeMs;

        // ring buffering
        samples_[writeIndex_] = frameTimeMs;
        writeIndex_ = (writeIndex_ + 1) % kMaxSamples;

        if (sampleCount_ < kMaxSamples)
        {
            ++sampleCount_;
        }

        elapsedSinceUpdate_ += deltaTimeSeconds;

        if (elapsedSinceUpdate_ >= kUpdateIntervalSeconds)
        {
            Recalculate();
            elapsedSinceUpdate_ = 0.0f;
        }
    }

    void FrameTimeStatistics::Recalculate()
    {
        if (sampleCount_ == 0)
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

    float FrameTimeStatistics::Percentile(std::array<float, kMaxSamples>::const_iterator first, std::size_t count,
                                          float percentile)
    {
        const float position = percentile * static_cast<float>(count - 1);
        const std::size_t index = static_cast<std::size_t>(std::ceil(position));

        // deref
        return *(first + index);
    }

    void FrameTimer::Tick()
    {
        const auto now = Clock::now();

        if (!initialized_)
        {
            prev_ = now;
            deltaSeconds_ = 0.0f;
            initialized_ = true;
            return;
        }

        deltaSeconds_ = std::chrono::duration<float>(now - prev_).count();
        prev_ = now;

        fpsAccumSeconds_ += deltaSeconds_;
        fpsFrameCount_++;

        // Average FPS over a 0.5-second interval
        if (fpsAccumSeconds_ >= 0.5f)
        {
            fps_ = static_cast<float>(fpsFrameCount_) / fpsAccumSeconds_;
            fpsAccumSeconds_ = 0.0f;
            fpsFrameCount_ = 0;
        }
    }
} // namespace ddknd::clock