#include "game/system/request/game_input_system.h"

#include "game/component/IntentComponent.h"
#include <ddknd/component/gfx_component.h>
#include <ddknd/input/input.h>
#include <ddknd/math/math.h>


#include "game/action/action.h"


namespace app::system
{
    void PlayerMovementIntentSystem::UpdateOne(app::component::RequestedMovementIntentComponent& out,
                                               const ::ddknd::input::ActionInputSystem& input)
    {
        using namespace ::app::action;
        if (!out.enabled)
        {
            out.active = false;
            out.moveAxis = {0.0f, 0.0f};
            return;
        }
            
        ::ddknd::math::Vec2f dir{0.0f, 0.0f};

        if (input.IsDown(Action::MoveFoward))
        {
            dir.y() += 1.0f;
        }
        if (input.IsDown(Action::MoveBackward))
        {
            dir.y() -= 1.0f;
        }
        if (input.IsDown(Action::MoveLeft))
        {
            dir.x() -= 1.0f;
        }
        if (input.IsDown(Action::MoveRight))
        {
            dir.x() += 1.0f;
        }

        if (::ddknd::math::lengthSquared(dir) > ::ddknd::math::kEpsilonSq<float>)
        {
            out.moveAxis = ::ddknd::math::normalize(dir);
            out.active = true;
        }
        else
        {
            out.moveAxis = {0.0f, 0.0f};
            out.active = false;
        }
    }
} // namespace app::system