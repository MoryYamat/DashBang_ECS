#include "game/player/player_controller.h"

#include "game/component/IntentComponent.h"
#include "game/component/controller_component.h"
#include <ddknd/ecs/ecs.h>


namespace app::player
{
    ::ddknd::ecs::Entity CreateLocalPlayerController(::ddknd::ecs::World& world, const PlayerControllerSpawnDesc& desc)
    {
        auto& reg = world.GetRegistry();

        auto e = reg.Create();

        reg.AddComponent<app::component::PlayerControllerComponent>(e, app::component::PlayerControllerComponent{});

        reg.AddComponent<app::component::ControlledActorComponent>(
            e, app::component::ControlledActorComponent{.actor = desc.actor});

        reg.AddComponent<app::component::ControlledCameraRigComponent>(
            e, app::component::ControlledCameraRigComponent{.cameraRig = desc.cameraRig});

        // *********** Requested Intent Components ***********
        reg.AddComponent<app::component::RequestedMovementIntentComponent>(e);
        reg.AddComponent<app::component::RequestedCameraIntentComponent>(e);
        reg.AddComponent<app::component::CameraControllerSettingsComponent>(
            e, app::component::CameraControllerSettingsComponent{.lookSensitivityDeg = desc.cameraLookSensitivityDeg,
                                                                 .zoomSensitivity = desc.cameraZoomSensitivity,
                                                                 .invertY = desc.invertCameraY});
        return e;
    }
} // namespace app::player