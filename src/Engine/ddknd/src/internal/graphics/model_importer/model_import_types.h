#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "math/math.h"

// importer/loader intermediate products
namespace ddknd::graphics::internal::type
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
    
    enum class ChannelType { Unknown, T, R, S};

    struct ImportChannel
    {
        using Vec3f = ::ddknd::math::Vec3f;
        using Quatf = ::ddknd::math::Quatf;

        int targetNode = -1;
        ChannelType type = ChannelType::Unknown;

        std::vector<float> times;
        std::vector<Vec3f> v3;      //T/S
        std::vector<Quatf> vq;       // R
    };

    // glTF specification 
    struct ImportScene
    {
        std::string name;
        std::vector<int> rootNodes;
    };

    struct ImportNode
    {
        using TRS = ::ddknd::math::TRS;
        using Mat4f = ::ddknd::math::Mat4f;

        std::string name;

        int parent = -1;
        std::vector<int> children;

        bool hasMatrix = false;
        TRS local;
        Mat4f localMatrix;
        Mat4f globalMatrix;

        int mesh = -1;
        int skin = -1;

    };

    struct ImportMesh
    {
        std::string name;
        std::vector<int> primitives;
    };

    struct ImportPrimitive
    {
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;

        int material = -1;
    };

    struct ImportSkin
    {
        using Mat4f = ::ddknd::math::Mat4f;

        std::string name;

        int skeletonRootNode = -1;
        std::vector<int> jointNodes;
        std::vector<Mat4f> inverseBindMatrices;
    };

    struct ImportAnimation
    {
        std::string name;
        float duration = 0.0f;
        std::vector<ImportChannel> channels;
    };

    struct ModelImportData
    {
        int defaultScene = -1;

        std::vector<ImportScene> scenes;
        std::vector<ImportNode> nodes;
        std::vector<ImportMesh> meshes;
        std::vector<ImportPrimitive> primitives;
        std::vector<ImportSkin> skins;
        std::vector<ImportAnimation> animations;
    };
}





