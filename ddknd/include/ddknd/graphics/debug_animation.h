#pragma once

#include <ddknd/component/component_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>

#include <ddknd/math/math.h>

// fwd
namespace ddknd::animation
{

    namespace types
    {
        struct SkeletonResource;
    }

    struct Pose;
} // namespace ddknd::animation

namespace ddknd::graphics
{
    class DebugDrawList;
}

namespace ddknd::animation::debug
{
    void TestAnimatorSystemInit(const animation::types::SkeletonResource& skeleton, animation::Pose& pose);
    void TestAnimatorSystemUpdate(const animation::types::SkeletonResource& skeleton, animation::Pose& pose,
                                  graphics::DebugDrawList& draw);

    struct SkeletonDebugDrawSystem
    {
        static void UpdateOne(const ::ddknd::component::SkinnedModelComponent& modelComp,
                              const ::ddknd::component::PoseComponent& poseComp,
                              const ::ddknd::component::TransformComponent& transformComp,
                              const ::ddknd::graphics::GraphicsAssetStore& graphicsStore,                                            
                              const ::ddknd::math::Vec4f& color,
                              ::ddknd::graphics::DebugDrawList& debugDraw);
    };

    void DrawSkeleton(const ::ddknd::animation::types::SkeletonResource& skeleton, const ::ddknd::animation::Pose& pose,
                      const ::ddknd::math::Mat4f& modelMatrix, const ::ddknd::math::Vec4f& color, ::ddknd::graphics::DebugDrawList& debugDraw);
} // namespace ddknd::animation::debug