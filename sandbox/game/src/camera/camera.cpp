#include "game/camera/camera.h"

#include <ddknd/ecs/ecs.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/math/math.h>

namespace
{
    ::ddknd::math::Vec3f CalcForawrdVector(const ::ddknd::math::Vec3f& target, const ::ddknd::math::Vec3f& pos)
    {
        ::ddknd::math::Vec3f forward = target - pos;
        if (::ddknd::math::lengthSquared(forward) <= ::ddknd::math::kEpsilonSq<float>)
        {
            forward = {0.0f, 0.0f, -1.0f};
        }
        else
        {
            forward = ::ddknd::math::normalize(forward);
        }

        return forward;
    }
} // namespace

namespace app::camera
{
    ::ddknd::ecs::Entity CreateCameraEntity(::ddknd::ecs::World& world, const CameraEntityDesc& desc)
    {
        auto e = world.Create();

        ::ddknd::component::TransformComponent transform{};
        transform.localTRS.translation = desc.position;

        world.GetRegistry().AddComponent<::ddknd::component::TransformComponent>(e, transform);

        world.GetRegistry().AddComponent<::ddknd::component::CameraProjectionComponent>(
            e, ::ddknd::component::CameraProjectionComponent{.projectionType =
                                                                 ddknd::component::ProjectionType::Perspective,
                                                             .nearZ = desc.nearZ,
                                                             .farZ = desc.farZ});

        ::ddknd::math::Vec3f forward = CalcForawrdVector(desc.lookTarget, desc.position);

        world.GetRegistry().AddComponent<::ddknd::component::CameraLookComponent>(
            e, ::ddknd::component::CameraLookComponent{.forward = forward, .up = desc.up});

        world.GetRegistry().AddComponent<::ddknd::component::CameraMatricesComponent>(e);

        if (desc.mainCamera)
        {
            world.GetRegistry().AddComponent<::ddknd::component::MainCameraTag>(e);
        }

        std::cerr << "ent pos " << desc.position << "\n";
        std::cerr << "ent look " << forward << "\n";
        return e;
    }
} // namespace app::camera