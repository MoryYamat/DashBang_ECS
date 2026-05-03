#include "math/math.h"

#include <numbers>

namespace ddknd::math
{
    float degToRad(float degree)
    {
        return degree * (std::numbers::pi_v<float> / 180.0f);
    }
}