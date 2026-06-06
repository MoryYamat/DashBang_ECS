#include <ddknd/camera/system_camera.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/test_component.h>

namespace ddknd::system
{
    void CameraSystem::UpdateDebugCamera(const component::TransformComponent& transform,
                                    const component::DebugCameraControllerComponent& debug,
                                    component::CameraComponent& camera)
    {
        const auto& pos = transform.localTRS.translation;
        const auto target = pos + debug.forward;

        camera.view = math::LookAtOpenGLRH(pos, target, debug.worldUp);

        if (camera.projectionType == component::ProjectionType::Perspective)
        {
            camera.proj = math::PerspectiveOpenGLRH(camera.fovYRad, camera.aspect, camera.nearZ, camera.farZ);
        }
        else
        {
            const float halfH = camera.orthoHeight * 0.5f;
            const float halfW = halfH * camera.aspect;

            camera.proj = math::OrthographicOpenGLRH(-halfW, halfW, -halfH, halfH, camera.nearZ, camera.farZ);
        }

        camera.viewProj = camera.proj * camera.view;
    }
} // namespace ddknd::system