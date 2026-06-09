#pragma once

#include <ddknd/math/math.h>

namespace ddknd::component
{
    struct VelocityComponent
    {
        using Vec3f = ::ddknd::math::Vec3f;

        Vec3f linear{0.0f, 0.0f, 0.0f};
        Vec3f angular{0.0f, 0.0f, 0.0f};
    };
}