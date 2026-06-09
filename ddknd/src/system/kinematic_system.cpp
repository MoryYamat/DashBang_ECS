#include <ddknd/system/kinematic_system.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/movement_component.h>


#include <ddknd/math/math.h>

namespace ddknd::system
{
    void KinematicSystem::UpdateOne(component::TransformComponent& transform,
                                    const component::VelocityComponent& velocity, const float dt)
    {
        if (math::lengthSquared(velocity.linear) <= math::kEpsilonSq<float>)
        {
            return;
        }

        transform.localTRS.translation += velocity.linear * dt;
        transform.dirty = true;
    }
} // namespace ddknd::system