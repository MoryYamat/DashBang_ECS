#include "ddknd/camera/debug_camera.h"

#include <algorithm>

#include "ddknd/component/gfx_component.h"
#include "ddknd/component/debug_camera_component.h"     // required for debug_ member
#include "ddknd/input/input.h"
#include "ddknd/math/math.h"


namespace ddknd::debug
{
    void DebugCameraController::Update(float dt)
    {
        using namespace ::ddknd::math;
        using Key = ::ddknd::input::Key;

        const auto& mouse = input_.Mouse();

        auto& pos = transform_.localTRS.translation;

        debug_.yawDeg += static_cast<float>(mouse.deltaX) * debug_.sensitivity;
        debug_.pitchDeg -= static_cast<float>(mouse.deltaY) * debug_.sensitivity;

        debug_.pitchDeg = std::clamp(debug_.pitchDeg, -89.0f, 89.0f);

        const float yawRad = degToRadf(debug_.yawDeg);
        const float pitchRad = degToRadf(debug_.pitchDeg);

        Vec3f forward = normalize(ComputeForwardVec(yawRad, pitchRad));
        Vec3f right = normalize(cross(forward, debug_.look.up));

        const float step = debug_.moveSpeed * dt;

        if (input_.IsKeyDown(Key::W))
        {
            pos += forward * step;
        }
        if (input_.IsKeyDown(Key::S))
        {
            pos -= forward * step;
        }
        if (input_.IsKeyDown(Key::A))
        {
            pos -= right * step;
        }
        if (input_.IsKeyDown(Key::D))
        {
            pos += right * step;
        }

        debug_.look.forward = forward;
        transform_.dirty = true;
    }
} // namespace ddknd::debug