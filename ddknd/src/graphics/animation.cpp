#include "ddknd/graphics/animation.h"

#include "ddknd/graphics/type/animation_types.h"
#include "ddknd/math/math.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace
{
    void ComputeBoneGlobal(ddknd::animation::types::Pose& pose, std::size_t boneIndex,
                           std::vector<std::uint8_t>& computed,
                           const ddknd::animation::types::SkeletonResource& skeleton)
    {
        if (computed[boneIndex] == 1)
        {
            return;
        }

        const int parent = skeleton.bones[boneIndex].parent;
        if (parent < 0)
        {
            pose.globalMatrices[boneIndex] = pose.localMatrices[boneIndex];
        }
        else
        {
            ComputeBoneGlobal(pose, static_cast<std::size_t>(parent), computed, skeleton);

            pose.globalMatrices[boneIndex] = pose.globalMatrices[parent] * pose.localMatrices[boneIndex];
        }

        computed[boneIndex] = 1;
    }

    int FindKeyFrame(const std::vector<float>& times, float t)
    {
        if (times.size() <= 1)
        {
            return 0;
        }

        auto it = std::lower_bound(times.begin(), times.end(), t);

        if (it == times.begin())
        {
            return 0;
        }

        if (it == times.end())
        {
            return static_cast<int>(times.size()) - 2;
        }

        return static_cast<int>(it - times.begin()) - 1;
    }

    float ComputeAlpha(const std::vector<float>& times, int index, float t)
    {
        if (times.size() <= 1)
        {
            return 0.f;
        }

        const float t0 = times[index];
        const float t1 = times[index + 1];
        const float dt = t1 - t0;
        if (dt <= 0.0f)
        {
            return 0.0f;
        }

        return (t - t0) / dt;
    }

    using Vec3f = ::ddknd::math::Vec3f;
    Vec3f Lerp(const Vec3f& a, const Vec3f& b, float t)
    {
        return a * (1.0f - t) + b * t;
    }

    using Quatf = ::ddknd::math::Quatf;
    Quatf Slerp(Quatf a, Quatf b, float t)
    {
        float cosTheta = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;

        if (cosTheta < 0.0f)
        {
            b.w = -b.w;
            b.x = -b.x;
            b.y = -b.y;
            b.z = -b.z;
            cosTheta = -cosTheta;
        }

        // Clamp floating-point error before passing the value to acos().
        cosTheta = std::clamp(cosTheta, 0.0f, 1.0f);

        if (cosTheta > 1.0f - ddknd::math::kEpsilon<float>)
        {
            Quatf out{a.w * (1.0f - t) + b.w * t, a.x * (1.0f - t) + b.x * t, a.y * (1.0f - t) + b.y * t,
                      a.z * (1.0f - t) + b.z * t};

            out.Normalize();
            return out;
        }

        const float theta = std::acos(cosTheta);
        const float sinTheta = std::sin(theta);

        const float w0 = std::sin((1.0f - t) * theta) / sinTheta;
        const float w1 = std::sin(t * theta) / sinTheta;

        Quatf out{a.w * w0 + b.w * w1, a.x * w0 + b.x * w1, a.y * w0 + b.y * w1, a.z * w0 + b.z * w1};

        out.Normalize();
        return out;
    }

    Vec3f SampleVec3(const std::vector<float>& times, const std::vector<Vec3f>& values, float time)
    {
        if (times.empty() || values.empty())
        {
            return Vec3f::Zero();
        }

        if (times.size() == 1 || values.size() == 1)
        {
            return values.front();
        }

        const int i = FindKeyFrame(times, time);
        const float alpha = ComputeAlpha(times, i, time);

        return Lerp(values[i], values[i + 1], alpha);
    }

    Quatf SampleQuat(const std::vector<float>& times, const std::vector<Quatf>& values, float time)
    {
        if (times.empty() || values.empty())
        {
            return Quatf::Identity();
        }

        if (times.size() == 1 || values.size() == 1)
        {
            return values.front();
        }

        const int i = FindKeyFrame(times, time);
        const float alpha = ComputeAlpha(times, i, time);

        return Slerp(values[i], values[i + 1], alpha);
    }

    ddknd::math::Mat4f MakeLocalMatrixForBone(const ddknd::animation::types::SkeletonResource& skeleton,
                                              std::size_t boneIndex, const ddknd::math::TRS& trs)
    {
        auto local = trs.ToMatrix();

        // Apply the skeleton-root node transform that is not stored as a bone.
        if (skeleton.bones[boneIndex].parent < 0)
        {
            return skeleton.skeletonRootTransform * local;
        }

        return local;
    }
} // namespace

namespace ddknd::animation
{
    void AnimatorSystem::InitializePose(types::Pose& pose, const types::SkeletonResource& skeleton)
    {

        const std::size_t boneCount = skeleton.bones.size();

        pose.localTRS.resize(boneCount);
        pose.localMatrices.resize(boneCount);

        for (std::size_t i = 0; i < boneCount; ++i)
        {
            pose.localTRS[i] = skeleton.bones[i].bindLocalTRS;

            pose.localMatrices[i] = MakeLocalMatrixForBone(skeleton, i, pose.localTRS[i]);
        }

        UpdateGlobalPose(pose, skeleton);
    }
    
    void AnimatorSystem::UpdateAnimationState(types::AnimationState& state, const types::AnimationClipResource& clip,
                                              float deltaTime)
    {
        if (clip.duration <= 0.0f)
        {
            return;
        }

        state.time += deltaTime * state.speed;

        if (state.loop)
        {
            state.time = std::fmod(state.time, clip.duration);

            if (state.time < 0.0f)
            {
                state.time += clip.duration;
            }
        }
        else
        {
            if (state.time < 0.0f)
            {
                state.time = 0.0f;
            }

            if (state.time > clip.duration)
            {
                state.time = clip.duration;
            }
        }
    }

    void AnimatorSystem::UpdatePose(types::Pose& pose, 
                               const types::AnimationState& state,
                               const types::SkeletonResource& skeleton, 
                               const types::AnimationClipResource& clip
                               )
    {
        SampleAnimation(pose, skeleton, clip, state.time);
    }


    void AnimatorSystem::UpdateGlobalPose(types::Pose& pose, const types::SkeletonResource& skeleton)
    {
        const std::size_t boneCount = skeleton.bones.size();

        pose.globalMatrices.resize(boneCount);
        pose.skinMatrices.resize(boneCount);

        // TODO: Reuse the computed-state buffer instead of allocating it every update.
        std::vector<std::uint8_t> computed(boneCount, 0);

        for (std::size_t i = 0; i < boneCount; ++i)
        {
            ComputeBoneGlobal(pose, i, computed, skeleton);
        }

        for (std::size_t i = 0; i < boneCount; ++i)
        {
            // Transform vertices from bind-pose model space to current model space.
            pose.skinMatrices[i] = pose.globalMatrices[i] * skeleton.bones[i].inverseBindMatrix;
        }
    }

    void AnimatorSystem::SampleAnimation(types::Pose& pose, 
                                         const types::SkeletonResource& skeleton,
                                         const types::AnimationClipResource& clip,
                                         const float time)
    {
        const std::size_t boneCount = skeleton.bones.size();

        pose.localTRS.resize(boneCount);
        pose.localMatrices.resize(boneCount);

        // 1. Initialize with the bind pose TRS.
        for (std::size_t i = 0; i < boneCount; ++i)
        {
            pose.localTRS[i] = skeleton.bones[i].bindLocalTRS;
        }

        // 2. Apply the animation channels.
        for (const auto& ch : clip.channels)
        {
            if (ch.bone < 0 || ch.bone >= static_cast<int>(boneCount))
            {
                continue;
            }

            auto& trs = pose.localTRS[ch.bone];

            switch (ch.target)
            {
            case types::ChannelTarget::Translation:
                trs.translation = SampleVec3(ch.times, ch.vec3Values, time);
                break;

            case types::ChannelTarget::Rotation:
                trs.rotation = SampleQuat(ch.times, ch.quatValues, time);
                break;

            case types::ChannelTarget::Scale:
                trs.scale = SampleVec3(ch.times, ch.vec3Values, time);
                break;
            case types::ChannelTarget::Unknown:
                assert(false && "Unknown animation channel target.");
                break;
            }
        }

        // 3. Convert local TRS values to matrices.
        for (std::size_t i = 0; i < boneCount; ++i)
        {
            pose.localMatrices[i] = MakeLocalMatrixForBone(skeleton, i, pose.localTRS[i]);
        }
        UpdateGlobalPose(pose, skeleton);
    }
} // namespace ddknd::animation