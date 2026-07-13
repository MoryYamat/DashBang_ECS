#include "ddknd/graphics/debug_animation.h"
#include "ddknd/graphics/animation.h"
#include "ddknd/graphics/gfx_type.h"

#include "ddknd/graphics/renderer.h"
#include "ddknd/math/math.h"

#include <ddknd/component/gfx_component.h>
#include <ddknd/graphics/gfx_asset_loader.h>


namespace ddknd::animation::debug
{
    void SkeletonDebugDrawSystem::UpdateOne(::ddknd::graphics::DebugDrawList& debugDraw,
                                            const ::ddknd::component::SkinnedModelComponent& modelComp,
                                            const ::ddknd::component::PoseComponent& poseComp,
                                            const ::ddknd::component::TransformComponent& transformComp,
                                            const ::ddknd::graphics::GraphicsAssetStore& graphicsStore,                                            
                                            const ::ddknd::math::Vec4f& color)
    {
        const auto* model = graphicsStore.TryGet(modelComp.model);
        if (!model || !model->skeleton)
        {
            return;
        }

        ::ddknd::animation::debug::DrawSkeleton(debugDraw, *model->skeleton, poseComp.pose, transformComp.worldMatrix, color);
    }

    void TestAnimatorSystemInit(const types::SkeletonResource& skeleton, animation::types::Pose& pose)
    {
        ::ddknd::animation::AnimatorSystem::InitializePose(pose, skeleton);
    }

    void TestAnimatorSystemUpdate(const animation::types::SkeletonResource& skeleton, animation::types::Pose& pose,
                                  graphics::DebugDrawList& draw)
    {
        using namespace math;
        for (std::size_t i = 0; i < skeleton.bones.size(); i++)
        {
            const int parent = skeleton.bones[i].parent;
            if (parent < 0)
                continue;
            const auto a = ExtractTranslation(pose.globalMatrices[parent]);
            const auto b = ExtractTranslation(pose.globalMatrices[i]);

            math::Vec4f color{0.0f, 1.0f, 1.0f, 1.0f};
            draw.Line(a, b, color);
        }
    }

    void DrawSkeleton(::ddknd::graphics::DebugDrawList& debugDraw, 
                      const ::ddknd::animation::types::SkeletonResource& skeleton, 
                      const ::ddknd::animation::types::Pose& pose,
                      const ::ddknd::math::Mat4f& modelMatrix, 
                      const ::ddknd::math::Vec4f& color)
    {
        const auto boneCount = skeleton.bones.size();

        if(pose.globalMatrices.size() < boneCount)
        {
            return;
        }

        for(std::size_t i = 0; i < boneCount; ++i)
        {
            const auto& bone = skeleton.bones[i];

            if(bone.parent < 0)
            {
                continue;
            }

            const auto parentIndex = static_cast<std::size_t>(bone.parent);

            if(parentIndex >= boneCount)
            {
                continue;
            }

            const auto childLocal = 
                    ::ddknd::math::ExtractTranslation(pose.globalMatrices[i]);

            const auto parentLocal=
                    ::ddknd::math::ExtractTranslation(pose.globalMatrices[parentIndex]);

            const auto childWorld =
                    ddknd::math::TransformPoint(modelMatrix, childLocal);

            const auto parentWorld =
                    ddknd::math::TransformPoint(modelMatrix, parentLocal);

            debugDraw.Line(
                parentWorld,
                childWorld,
                color
            );
        }


    }
} // namespace ddknd::animation::debug