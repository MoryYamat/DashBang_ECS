#pragma once

#include <ddknd/component/component_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>

#include <ddknd/math/math.h>


namespace ddknd::animation::debug
{
    void TestAnimatorSystemInit(animation::types::Pose& pose, const animation::types::SkeletonResource& skeleton);
    void TestAnimatorSystemUpdate(graphics::debug::DebugDrawList& draw, const animation::types::SkeletonResource& skeleton, animation::types::Pose& pose);

    struct SkeletonDebugDrawSystem
    {
        static void UpdateOne(graphics::debug::DebugDrawList& debugDraw,
                              const ::ddknd::component::SkinnedModelComponent& modelComp,
                              const ::ddknd::component::PoseComponent& poseComp,
                              const ::ddknd::component::TransformComponent& transformComp,
                              const ::ddknd::graphics::GraphicsAssetStore& graphicsStore,                                            
                              const ::ddknd::math::Vec4f& color);
    };

    void DrawSkeleton(graphics::debug::DebugDrawList& debugDraw, 
                      const ddknd::animation::types::SkeletonResource& skeleton, 
                      const ddknd::animation::types::Pose& pose,
                      const ddknd::math::Mat4f& modelMatrix, 
                      const ddknd::math::Vec4f& color);
} // namespace ddknd::animation::debug