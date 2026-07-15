#pragma once

#include "ddknd/asset/asset_id.h"
#include "ddknd/asset/asset_tag.h"

#include "ddknd/graphics/type/gpu_types.h"
#include "ddknd/graphics/type/animation_types.h"

#include "ddknd/math/math.h"

#include <optional>

namespace ddknd::graphics::types
{
    struct GlyphResource
    {
      private:
        using Vec2f = math::Vec2f;

      public:
        Vec2f uv0;
        Vec2f uv1;
        Vec2f size;
        Vec2f offset; // offset from the text pen position.
        float advance = 0.0f;
    };

        struct TextureSlot
    {
        std::optional<ddknd::asset::AssetID<::ddknd::asset::tag::TextureTag>> texture;
        std::uint32_t texCoord = 0;
    };


    struct NormalTextureSlot
    {
        std::optional<ddknd::asset::AssetID<::ddknd::asset::tag::TextureTag>> texture;
        std::uint32_t texCoord = 0;
        float scale = 1.0f;
    };

    struct OcclusionTextureSlot
    {
        std::optional<ddknd::asset::AssetID<::ddknd::asset::tag::TextureTag>> texture;
        std::uint32_t texCoord = 0;
        float strength = 1.0f;
    };

    enum class AlphaMode
    {
        OPAQUE,
        MASK,
        BLEND
    };

    enum class TextureUsage
    {
        Unknown,
        FontAtlas,
        BaseColor,
        Normal,
        MetallicRoughness,
        Occlusion,
        Emissive,
    };

    struct MaterialResource
    {
        math::Vec4f baseColorFactor{1, 1, 1, 1};
        TextureSlot baseColorTexture;
        float metallicFactor = 1.0f;
        float roughnessFactor = 1.0f;
        TextureSlot metallicRoughnessTexture;

        NormalTextureSlot normalTexture;
        OcclusionTextureSlot occlusionTexture;

        TextureSlot emissiveTexture;
        math::Vec3f emissiveFactor{0, 0, 0};

        types::AlphaMode alphaMode = types::AlphaMode::OPAQUE;
        float alphaCutoff = 0.5f;
        bool doubleSided = false;
    };

    struct ShaderResource
    {
        GPUID<tag::ShaderProgramGPUTag> program;
    };

    struct TextureResource
    {
        GPUID<tag::TextureGPUTag> gpuTexture;

        std::uint32_t width = 0;
        std::uint32_t height = 0;
        std::uint32_t channels = 0;

        graphics::types::TextureFormat format = graphics::types::TextureFormat::RGBA8;
    };

    struct PrimitiveResource
    {
        GPUID<tag::PrimitiveTag> primitive;
        std::uint32_t vertexCount = 0;
        std::uint32_t indexCount = 0;

        std::uint32_t materialIndex = 0;
    };

    struct ModelRenderResource
    {
        int sourceScene = 0;
        std::vector<PrimitiveResource> primitives;
        std::vector<MaterialResource> materials;
        std::optional<ddknd::animation::types::SkeletonResource> skeleton;
        std::vector<ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip>> clips; 
    };

    struct FontResource
    {
        GPUID<tag::TextureGPUTag> atlas;

        int atlasWidth = 0;
        int atlasHeight = 0;

        int firstCodepoint = 32;
        int glyphCount = 96;

        float pixelHeight = 18.0f;

        std::vector<::ddknd::graphics::types::GlyphResource> glyphs;
    };
}