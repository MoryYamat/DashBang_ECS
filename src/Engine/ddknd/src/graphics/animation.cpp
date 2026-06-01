#include "graphics/animation.h"

#include "graphics/gfx_type.h"

#include <cstddef>

namespace
{
    void ComputeBoneGlobal(const ddknd::animation::types::SkeletonResource& skeleton, ddknd::animation::Pose& pose,
                           std::size_t boneIndex, std::vector<bool>& computed)
    {
        if (computed[boneIndex])
            return;

        const int parent = skeleton.bones[boneIndex].parent;

        if (parent < 0)
        {
            pose.globalMatrices[boneIndex] = pose.localTransforms[boneIndex];
        }
        else
        {
            ComputeBoneGlobal(skeleton, pose, static_cast<std::size_t>(parent), computed);

            pose.globalMatrices[boneIndex] = pose.globalMatrices[parent] * pose.localTransforms[boneIndex];
        }

        computed[boneIndex] = true;
    }
} // namespace

namespace ddknd::animation
{
    void AnimaterSystem::InitializePose(const types::SkeletonResource& skeleton, Pose& pose)
    {
        const std::size_t boneCount = skeleton.bones.size();

        pose.localTransforms.resize(boneCount);
        pose.globalMatrices.resize(boneCount);
        pose.skinMatrices.resize(boneCount);

        for (std::size_t i = 0; i < boneCount; ++i)
        {
            pose.localTransforms[i] = skeleton.bones[i].bindLocalMatrix;
        }

        std::vector<bool> computed(boneCount, false);

        for (std::size_t i = 0; i < boneCount; ++i)
        {
            ComputeBoneGlobal(skeleton, pose, i, computed);
        }

        for (std::size_t i = 0; i < boneCount; ++i)
        {
            pose.skinMatrices[i] = pose.globalMatrices[i] * skeleton.bones[i].inverseBindMatrix;
        }
    }
    
    void SampleAnimation(const types::AnimationClipResource& clip, float time, Pose& pose)
    {

    }
} // namespace ddknd::animation