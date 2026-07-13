#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "ddknd/math/math.h"

// importer/loader intermediate products
namespace ddknd::graphics::type
{
    // for loading
    struct Vertex
    {
        using Vec2f = ::ddknd::math::Vec2f;
        using Vec3f = ::ddknd::math::Vec3f;
        using Vec4f = ::ddknd::math::Vec4f;
        using uVec4 = ::ddknd::math::uVec4;

        Vec3f pos;
        Vec3f normal;
        Vec2f texCoords;
        Vec4f tangent;// (x,y,z,w): xyz + w(sign)

        uVec4 joints;
        Vec4f weights;

    };

    struct Bone
    {
        using Mat4f = ::ddknd::math::Mat4f;
        using TRS = ::ddknd::math::TRS;
        Mat4f invBind = Mat4f::Identity();
        TRS bindLocal;

        int parent = -1;

        int nodeIndex = -1;     // glTF node[]
        std::string name;       // for debug
    };

    struct Skeleton
    {
        std::vector<Bone> bones;

        std::unordered_map<int, int> nodeToBone;

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

    struct MeshGeometry
    {
        std::vector<Vertex> vertices;
        std::vector<std::uint64_t> indices;
    };

    // Discarded after loading
    struct ModelImportData
    {
        using Mat4f = ::ddknd::math::Mat4f;

        std::vector<MeshGeometry> meshes;

        Mat4f meshBindGlobal = Mat4f::Identity();
        Mat4f skeletonRootBindGlobal = Mat4f::Identity();

        Skeleton skeleton;
        std::vector<AnimationClip> clips;
    };
}


namespace ddknd::animation::asset
{
    struct ModelAnimationResource
    {
        ::ddknd::graphics::type::Skeleton skeleton;
        std::vector<::ddknd::graphics::type::AnimationClip> clips;
    };
}//namespace ddknd::animation::asset