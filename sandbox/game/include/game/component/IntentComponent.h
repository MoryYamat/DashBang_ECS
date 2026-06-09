#pragma once
#include <ddknd/math/math.h>

namespace app::component
{
    struct PlayerControlComponent
    {
        // player-controlled entity marker
    };

    struct RequestedMovementIntentComponent
    {
        using Vec3f = ::ddknd::math::Vec3f;

        Vec3f direction{0.0f, 0.0f, 0.0f};
        bool active = false;
    };

    struct MovementIntentComponent
    {
        using Vec3f = ::ddknd::math::Vec3f;

        Vec3f direction{0.0f, 0.0f, 0.0f};
        bool active = false;
    };
}// namespace app::component