/*
.glb 形式の パース 用 データ構造
*/

#pragma once

#include <vector>
#include <string>
#include "ddknd/math/math.h"

// importer/loader intermediate products
namespace ddknd::graphics::internal::types
{
    
    /*size 80byte*/
    // for loading
    struct Vertex
    {
        using Vec2f = ::ddknd::math::Vec2f;     // 8byte
        using Vec3f = ::ddknd::math::Vec3f;     // 12byte
        using Vec4f = ::ddknd::math::Vec4f;     // 16byte
        using uVec4 = ::ddknd::math::uVec4;     // 16byte

        Vec3f pos;
        Vec3f normal;
        Vec2f texCoords;
        Vec4f tangent;// (x,y,z,w): xyz + w(sign)

        uVec4 joints;
        Vec4f weights;
    };    
    
    enum class ChannelType : std::uint8_t { Unknown, T, R, S};

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
        TRS localTRS;
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





