#pragma once
#include <ddknd/ecs/entity/entity.h>
#include <ddknd/math/math.h>

namespace app::component
{
    struct RequestedMovementIntentComponent
    {
        using Vec2f = ::ddknd::math::Vec2f;

        bool enabled = true;// Is it possible to accept input?
        bool active = false;// Whether a movement request was made within the frame

        //Vec3f direction{0.0f, 0.0f, 0.0f};
        Vec2f moveAxis{0.0f, 0.0f};
    };

    struct MovementIntentComponent
    {
        using Vec3f = ::ddknd::math::Vec3f;

        Vec3f direction{0.0f, 0.0f, 0.0f};
        bool active = false;
    };
}// namespace app::component