#include "math/math.h"

#include <numbers>
#include <cmath>

namespace ddknd::math
{
    float degToRadf(float degree)
    {
        return degree * (std::numbers::pi_v<float> / 180.0f);
    }

    Vec3f ComputeForawrdVec(float yawRad, float pitchRad)
    {
        Vec3f forward;
        forward[0] = std::cos(yawRad) *  std::cos(pitchRad);
        forward[1] = std::sin(pitchRad);
        forward[2] = std::sin(yawRad) * std::cos(pitchRad);

        return forward;
    }
}