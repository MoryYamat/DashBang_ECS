#include "ddknd/camera/debug_camera.h"

#include <algorithm>

#include "ddknd/component/test_component.h"
#include "ddknd/input/input.h"

#include "ddknd/math/math.h"

// helpers
namespace
{

}

namespace ddknd::debug
{
    void DebugCameraController::Update()
    {
        using namespace ::ddknd::math;
        using Key = ::ddknd::input::Key;

        const auto& mouse = input_.Mouse();
        auto& pos = cam_.pos;
        auto& target = cam_.target;
        const auto& up = cam_.up;

        // Update Target Vector
        const auto& sensitivity = cam_.sensitivty;
        auto& yaw = cam_.yaw;
        auto& pitch = cam_.pitch;
        yaw += static_cast<float>(mouse.deltaX) * sensitivity;
        pitch -= static_cast<float>(mouse.deltaY) * sensitivity;
        
        pitch = std::clamp(pitch, -89.0f, 89.0f);

        const float yawRad = math::degToRadf(yaw);
        const float pitchRad = math::degToRadf(pitch);

        math::Vec3f forward{math::normalize(math::ComputeForawrdVec(yawRad, pitchRad))};
        math::Vec3f right{math::normalize(math::cross(forward, up))};

        if (input_.isPressing(Key::W))
        {
            pos += forward * 0.1;
        }
        if (input_.isPressing(Key::S))
        {
            pos -= forward * 0.1;
        }
        if (input_.isPressing(Key::A))
        {
            pos -= right * 0.1;
        }
        if (input_.isPressing(Key::D))
        {
            pos += right * 0.1;
        }

        target = pos + forward;
    }
} // namespace ddknd::debug