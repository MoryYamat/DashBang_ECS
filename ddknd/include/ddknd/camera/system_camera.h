#pragma once

namespace ddknd::component
{
    struct TransformComponent;
    struct CameraComponent;
    struct DebugCameraControllerComponent;
}

namespace ddknd::system
{
    class CameraSystem
    {
        public:
            static void UpdateDebugCamera
            (const component::TransformComponent& transform,
                const component::DebugCameraControllerComponent& debug,
                component::CameraComponent& camera);
    };
}