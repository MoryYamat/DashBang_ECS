#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <string>
#include "math/math.h"

// importer/loader intermediate products
namespace ddknd::graphics::type
{
    struct Vertex
    {
        using Vec2f = ::ddknd::math::Vec2f;
        using Vec3f = ::ddknd::math::Vec3f;
        using Vec4f = ::ddknd::math::Vec4f;
        using uVec4 = ::ddknd::math::uVec4;

        uVec4 joints;
        Vec4f weights;

        Vec3f pos;
        Vec3f norm;
        Vec2f texCoords;
        Vec3f tangent;
        Vec3f bitangent;

    };

    struct Bone
    {
        using Mat4f = ::ddknd::math::Mat4f;
        using TRS = ::ddknd::math::TRS;
        Mat4f invBind = Mat4f::Identity();
        TRS bindLocal;

        std::optional<int> parent;
        std::string name;
    };

    struct Skeleton
    {
        std::vector<Bone> bones;

        std::unordered_map<std::string, int> nameToBone;
    };

    enum class ChannelType {T, R, S};

    struct Channel
    {
        using Vec3f = ::ddknd::math::Vec3f;
        using Quatf = ::ddknd::math::Quatf;

        int bone = -1;
        ChannelType type;
        std::vector<float> times;
        std::vector<Vec3f> v3;      //T/S
        std::vector<Quatf> vq;       // R
    };

    struct AnimationClip
    {
        std::string name;
        float duration = 0.0f;
        std::vector<Channel> channels;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;

        bool hasIndices = false;
    };

    // Discarded after loading
    struct ModelImportData
    {
        std::vector<Mesh> meshes;
        Skeleton skeleton;
        std::vector<AnimationClip> clips;
    };
}


namespace ddknd::animation::asset
{
    // struct SkeletonResource
    // {
    //     ::ddknd::graphics::type::Skeleton skeleton;
    // };

    // struct AnimationClipResource
    // {
    //     ::ddknd::graphics::type::AnimationClip clip;
    // };

    struct ModelAnimationResource
    {
        ::ddknd::graphics::type::Skeleton skeleton;
        std::vector<::ddknd::graphics::type::AnimationClip> clips;
    };
}//namespace ddknd::animation::asset