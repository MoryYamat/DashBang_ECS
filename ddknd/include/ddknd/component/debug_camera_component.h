#pragma once


#include <ddknd/component/gfx_component.h>


namespace ddknd::component
{
    struct DebugCameraControllerComponent
    {
        float yawDeg = -90.0f;
        float pitchDeg = 0.0f;

        float sensitivity = 0.1f;
        float moveSpeed = 5.0f;

        CameraLookComponent look{};
        CameraProjectionComponent projection{};
        CameraMatricesComponent matrices{};
    };
} // namespace ddknd::component