#include "game/system/input_system.h"

#include <ddknd/math/math.h>
#include <ddknd/input/input.h>
#include "game/component/IntentComponent.h"

#include "game/action/action.h"

namespace app::system
{
    void PlayerMovementIntentSystem::UpdateOne(app::component::RequestedMovementIntentComponent& movementIntent,
                                       const ::ddknd::input::ActionInputSystem& input)
    {
        using namespace ::app::action;
        if(!movementIntent.active)
            return;

        ::ddknd::math::Vec3f dir{0.0f, 0.0f, 0.0f};

        if(input.IsDown(Action::MoveFoward))
        {
            dir.z() -= 1.0f;
        }
        if(input.IsDown(Action::MoveBackward))
        {
            dir.z() += 1.0f;
        }
        if(input.IsDown(Action::MoveLeft))
        {
            dir.x() -= 1.0f;
        }
        if(input.IsDown(Action::MoveRight))
        {
            dir.x() += 1.0f;
        }

        if(::ddknd::math::lengthSquared(dir) > ::ddknd::math::kEpsilonSq<float>)
        {
            movementIntent.direction = ::ddknd::math::normalize(dir);
            movementIntent.active = true;
        }
        else
        {
            movementIntent.direction = {0.0f,0.0f,0.0f};
            movementIntent.active = false;
        }
    }
} // namespace app::system