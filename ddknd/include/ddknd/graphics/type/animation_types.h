#pragma once

#include <ddknd/asset/asset_id.h>
#include <ddknd/asset/asset_tag.h>
#include <ddknd/math/math.h>

#include <string>
#include <vector>

namespace ddknd::animation::types
{
    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;

    struct Pose
    {
        using TRS = ::ddknd::math::TRS;
        using Mat4f = ::ddknd::math::Mat4f;

        std::vector<TRS> localTRS;
        std::vector<Mat4f> localMatrices;
        std::vector<Mat4f> globalMatrices;
        std::vector<Mat4f> skinMatrices;
    };

    struct AnimationState
    {
        AssetID<::ddknd::asset::tag::AnimationClip> clip;
        float time = 0.0f;
        float speed = 1.0f;
        bool loop = true;
    };

    struct Bone
    {
        using Mat4f = ::ddknd::math::Mat4f;
        using TRS = ::ddknd::math::TRS;

        int parent = -1;
        Mat4f inverseBindMatrix = Mat4f::Identity();
        Mat4f parentCorrection = Mat4f::Identity();
        Mat4f bindLocalMatrix = Mat4f::Identity();
        TRS bindLocalTRS{};
    };

    struct SkeletonResource
    {
        using Mat4f = ::ddknd::math::Mat4f;
        std::vector<Bone> bones;

        Mat4f skeletonRootTransform = Mat4f::Identity();
    };

    enum class ChannelTarget
    {
        Unknown,
        Translation,
        Rotation,
        Scale,
    };

    enum class Interpolation
    {
        Linear
    };

    struct AnimationChannel
    {
        using Vec3f = ::ddknd::math::Vec3f;
        using Quatf = ::ddknd::math::Quatf;

        int bone = -1;
        ChannelTarget target = ChannelTarget::Unknown;

        std::vector<float> times;
        std::vector<Vec3f> vec3Values;
        std::vector<Quatf> quatValues;

        Interpolation interpolation = Interpolation::Linear;
    };

    struct AnimationClipResource
    {
        std::string name;
        float duration = 0.0f;
        std::vector<AnimationChannel> channels;
    };
} // namespace ddknd::animation::types

