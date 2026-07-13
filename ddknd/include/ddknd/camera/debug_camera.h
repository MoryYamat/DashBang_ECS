#pragma once
#include "ddknd/component/component_fwd.h"
#include "ddknd/input/input_fwd.h"

namespace ddknd::debug
{
    /**
     * Holds non-owning references to the input state and camera components.
     */
    class DebugCameraController
    {
      public:
        DebugCameraController(const ddknd::input::DeviceInput& input,
                              ddknd::component::DebugCameraControllerComponent& debug,
                              ddknd::component::TransformComponent& tsr)
            : input_(input), debug_(debug), transform_(tsr)
        {
        }

        void Update(float dt);

      private:
        using Input = ::ddknd::input::DeviceInput;
        using DebugCameraControllerComponent = ::ddknd::component::DebugCameraControllerComponent;
        using TransformComponent = ::ddknd::component::TransformComponent;

      private:
        const Input& input_;
        DebugCameraControllerComponent& debug_;
        TransformComponent& transform_;
    };
} // namespace ddknd::debug