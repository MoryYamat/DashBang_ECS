#pragma once

#include <ddknd/component/component_fwd.h>

namespace ddknd::system
{
    class KinematicSystem
    {
        public: 
            static void UpdateOne(
                component::TransformComponent& transform,
                const component::VelocityComponent& velocity,
                const float dt
            );
    };
}