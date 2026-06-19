#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>


#include "ddknd/graphics/gfx_type.h"
#include "ddknd/math/math.h"


namespace ddknd::graphics::internal::types
{
    using ImportIndex = std::uint32_t;

    struct ImportImage
    {
        std::string name;
        std::string uri;
        std::optional<::ddknd::graphics::types::MimeType> mimeType;

        std::vector<std::uint8_t> encodedBytes; // PNG/JPEG's encoded bytes
    };

    struct ImportSampler
    {
        std::string name;
        std::optional<::ddknd::graphics::types::TextureFilter> magFilter; // 9728/9729
        std::optional<::ddknd::graphics::types::TextureFilter> minFilter; // 9728/9729/9984/9985/9986/9987
        ::ddknd::graphics::types::TextureWrap wrapS = ::ddknd::graphics::types::TextureWrap::Repeat;            // 33071/33648/10497
        ::ddknd::graphics::types::TextureWrap wrapT = ::ddknd::graphics::types::TextureWrap::Repeat;            // 33071/33648/10497
    };

    struct ImportTexture
    {
        std::string name;

        std::optional<ImportIndex> sampler; // index to sampler
        std::optional<ImportIndex> source;  // index to image
    };

    struct ImportTextureInfo
    {
        std::optional<ImportIndex> index;
        std::uint32_t texCoord = 0; // TEXCOORD_N
    };

    struct ImportPBRMetallicRoughness
    {
        using Vec4f = ::ddknd::math::Vec4f;
        Vec4f baseColorFactor{1, 1, 1, 1};
        ImportTextureInfo baseColorTexture;
        float metallicFactor = 1.0f;
        ImportTextureInfo metallicRoughnessTexture;
        float roughnessFactor = 1.0f;
    };

    struct ImportNormalTexture
    {
        std::optional<ImportIndex> index;
        std::uint32_t texCoord = 0;
        float scale = 1.0f;
    };

    struct ImportOcclusionTexture
    {
        std::optional<ImportIndex> index;
        std::uint32_t texCoord = 0;
        float strength = 1.0f;
    };

    struct ImportMaterial
    {
        using Vec4f = ::ddknd::math::Vec4f;
        using Vec3f = ::ddknd::math::Vec3f;
        std::string name;

        ImportPBRMetallicRoughness pbrMetallicRoughness;
        ImportNormalTexture normalTexture;
        ImportOcclusionTexture occlusionTexture;

        ImportTextureInfo emissiveTexture;
        Vec3f emissiveFactor = {0.0f, 0.0f, 0.0f};

        ::ddknd::graphics::types::AlphaMode alphaMode =
            ::ddknd::graphics::types::AlphaMode::OPAQUE; // OPAQUE / MASK / BLEND
        float alphaCutoff = 0.5f;
        bool doubleSided = false;
    };
} // namespace ddknd::graphics::internal::types