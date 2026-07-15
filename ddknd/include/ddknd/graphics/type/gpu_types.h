#pragma once

#include "ddknd/core/StrongID.h"
#include "ddknd/math/math.h"

#include <string>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>
#include <functional>

namespace ddknd::graphics::tag
{
    struct ShaderProgramGPUTag
    {
    };
    struct PrimitiveTag
    {
    };
    struct TextureGPUTag
    {
    };
    struct ScreenQuadBatchTag
    {
    };
    struct LineBatchTag
    {
    };
    struct ModelTag
    {
    };
} // namespace ddknd::graphics::tag

namespace ddknd::graphics::types
{
    template <typename GPUTag>
    using GPUID = ddknd::core::StrongID<GPUTag, std::uint32_t>;

    struct Vertex
    {
        using Vec2f = ::ddknd::math::Vec2f;
        using Vec3f = ::ddknd::math::Vec3f;
        using Vec4f = ::ddknd::math::Vec4f;
        using uVec4 = ::ddknd::math::uVec4;

        Vec3f pos;
        Vec3f normal;
        Vec2f texCoords;
        Vec4f tangent; // xyz: tangent direction, w: handedness

        uVec4 joints;
        Vec4f weights;
    };

    struct PrimitiveCreateData
    {
        std::vector<ddknd::graphics::types::Vertex> vertices;
        std::vector<std::uint32_t> indices;

        int materialIndex = -1;
    };

    struct PrimitiveKey
    {
        std::string fileKey; 
        int primitiveIndex = -1;

        bool operator==(const PrimitiveKey& o) const
        {
            return primitiveIndex == o.primitiveIndex && fileKey == o.fileKey;
        }
    };

    struct PrimitiveKeyHash
    {
        std::size_t operator()(const PrimitiveKey& k) const
        {
            std::size_t h1 = std::hash<std::string>{}(k.fileKey);
            std::size_t h2 = std::hash<int>{}(k.primitiveIndex);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };

    struct ScreenQuadVertex
    {
        math::Vec2f pos; // Screen-space position in pixels.
        math::Vec2f uv;
        math::Vec4f color;
    };

    struct LineVertex
    {
        math::Vec3f pos;
        math::Vec4f color;
    };

    enum class TextureFilter
    {
        Nearest,
        Linear,

        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class TextureWrap
    {
        ClampToEdge,
        MirroredRepeat,
        Repeat
    };

    enum class TextureFormat
    {
        Unknown,
        R8,

        RGB8,
        RGBA8,

        SRGB8,
        SRGBA8,
    };

    struct SamplerDesc
    {
        ::ddknd::graphics::types::TextureFilter minFilter = ::ddknd::graphics::types::TextureFilter::Linear;
        ::ddknd::graphics::types::TextureFilter magFilter = ::ddknd::graphics::types::TextureFilter::Linear;
        ::ddknd::graphics::types::TextureWrap wrapS = ::ddknd::graphics::types::TextureWrap::Repeat;
        ::ddknd::graphics::types::TextureWrap wrapT = ::ddknd::graphics::types::TextureWrap::Repeat;
    };

    struct Texture2DCreateDesc
    {
        std::uint32_t width;
        std::uint32_t height;
        TextureFormat format = TextureFormat::Unknown;
        std::span<const std::uint8_t> pixels;
        SamplerDesc sampler;
        bool generateMipmap;
    };
} // namespace ddknd::graphics::types