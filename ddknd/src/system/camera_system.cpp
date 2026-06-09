#include <ddknd/component/gfx_component.h>
#include <ddknd/system/camera_system.h>

#include <ddknd/math/math.h>

namespace ddknd::system
{
    void CameraMatricesSystem::UpdateOne(::ddknd::component::CameraMatricesComponent& matrices,
                              const ::ddknd::component::TransformComponent& transform,
                              const ::ddknd::component::CameraLookComponent& look,
                              const ::ddknd::component::CameraProjectionComponent& projection)
    {
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

    void CameraProjectionSystem::UpdateOne(::ddknd::component::CameraProjectionComponent& projection, const float aspect)
    {
        projection.aspect = aspect;
    }
} // namespace ddknd::system