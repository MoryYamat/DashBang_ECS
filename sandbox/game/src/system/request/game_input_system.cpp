#include "game/system/request/game_input_system.h"

#include "game/component/IntentComponent.h"
#include "game/component/controller_component.h"
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

    void PlayerCameraIntentSystem::UpdateOne(app::component::RequestedCameraIntentComponent& out,
                                             const app::component::CameraControllerSettingsComponent& settings,
                                             const ::ddknd::input::ActionInputSystem& input)
    {
        using namespace ::app::action;

        if (!out.enabled)
        {
            out.active = false;
            out.yawDeltaDeg = 0.0f;
            out.pitchDeltaDeg = 0.0f;
            out.zoomDelta = 0.0f;
            return;
        }

        const float lookX = input.GetValue(Action::CameraLookX);
        const float lookY = input.GetValue(Action::CameraLookY);
        const float zoom = input.GetValue(Action::CameraZoom);

        out.yawDeltaDeg = lookX * settings.lookSensitivityDeg;
        const float pitchSign = settings.invertY ? -1.0f : 1.0f;
        out.pitchDeltaDeg = lookY * settings.lookSensitivityDeg * pitchSign;

        out.zoomDelta = -zoom * settings.zoomSensitivity;

        out.active = std::abs(out.yawDeltaDeg) > 0.0001f || std::abs(out.pitchDeltaDeg) > 0.0001f ||
                     std::abs(out.zoomDelta) > 0.0001f;
    }

    void PlayerAttackIntentSystem::UpdateOne(app::component::RequestedAttackIntentComponent& out,
                                             const ::ddknd::input::ActionInputSystem& input)
    {
        using namespace app::action;

        if(!out.enabled)
        {
            out.active = false;
            return;
        }

        out.active = input.IsPressed(Action::Attack);
    }
} // namespace app::system