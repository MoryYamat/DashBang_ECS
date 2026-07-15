#include <ddknd/camera/system_debug_camera.h>

#include <ddknd/component/debug_camera_component.h>
#include <ddknd/component/gfx_component.h>

namespace ddknd::system
{
    void DebugCameraSystem::UpdateDebugCamera(const component::TransformComponent& transform,
                                      component::DebugCameraControllerComponent& camera)
    {
        auto& look = camera.look;
        auto& matrices = camera.matrices;
        auto& projection = camera.projection;

        const auto& pos = transform.localTRS.translation;
        const auto target = pos + look.forward;

        matrices.view = math::LookAtOpenGLRH(pos, target, look.up);

        if (projection.projectionType == component::ProjectionType::Perspective)
        {
            matrices.proj = math::PerspectiveOpenGLRH(projection.fovYRad, projection.aspect, projection.nearZ, projection.farZ);
        }
        else
        {
            const float halfH = projection.orthoHeight * 0.5f;
            const float halfW = halfH * projection.aspect;

            matrices.proj = math::OrthographicOpenGLRH(-halfW, halfW, -halfH, halfH, projection.nearZ, projection.farZ);
        }

        matrices.viewProj = matrices.proj * matrices.view;
    }
} // namespace ddknd::system