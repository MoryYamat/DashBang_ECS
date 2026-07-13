#include "internal/graphics/model_importer/glb_importer_debug.h"

#include "internal/graphics/model_importer/model_import_types.h"
#include "ddknd/graphics/gfx_type.h"


#include <cgltf.h>


namespace glType = ::ddknd::graphics::internal::types;
using ModelImportData = glType::ModelImportData;
using ImportNode = glType::ImportNode;
using ImportPrimitive = glType::ImportPrimitive;
using Vertex = glType::Vertex;
using ImportChannel = glType::ImportChannel;
using ChannelType = glType::ChannelType;

namespace ddknd::graphics::internal::debug
{
    const char* accessorTypeName(cgltf_type type)
    {
        switch (type)
        {
        case cgltf_type_scalar:
            return "SCALAR";
        case cgltf_type_vec2:
            return "VEC2";
        case cgltf_type_vec3:
            return "VEC3";
        case cgltf_type_vec4:
            return "VEC4";
        case cgltf_type_mat2:
            return "MAT2";
        case cgltf_type_mat3:
            return "MAT3";
        case cgltf_type_mat4:
            return "MAT4";
        default:
            return "UNKNOWN";
        }
    }
    const char* componentTypeName(cgltf_component_type type)
    {
        switch (type)
        {
        case cgltf_component_type_r_8:
            return "BYTE";
        case cgltf_component_type_r_8u:
            return "UNSIGNED_BYTE";
        case cgltf_component_type_r_16:
            return "SHORT";
        case cgltf_component_type_r_16u:
            return "UNSIGNED_SHORT";
        case cgltf_component_type_r_32u:
            return "UNSIGNED_INT";
        case cgltf_component_type_r_32f:
            return "FLOAT";
        default:
            return "UNKNOWN";
        }
    }
    void printAccessorInfo(const cgltf_accessor* accessor)
    {
        if (!accessor)
            return;

        std::cout << "accessor type      : " << accessorTypeName(accessor->type) << "\n";
        std::cout << "component type     : " << componentTypeName(accessor->component_type) << "\n";
        std::cout << "component count    : " << cgltf_num_components(accessor->type) << "\n";
        std::cout << "component size     : " << cgltf_component_size(accessor->component_type) << " bytes\n";
        std::cout << "element byte size  : " << cgltf_calc_size(accessor->type, accessor->component_type) << " bytes\n";
        std::cout << "count              : " << accessor->count << "\n";
    }

    void DebugTextureSlot(const ModelImportData& out, const char* label, const glType::ImportTextureInfo& info)
    {
        std::cerr << "  " << label << "\n";

        if (!info.index)
        {
            std::cerr << "    texture: none\n";
            return;
        }

        const auto textureIndex = *info.index;

        std::cerr << "    texture index=" << textureIndex << "\n";
        std::cerr << "    texCoord=" << info.texCoord << "\n";

        if (textureIndex >= out.textures.size())
        {
            std::cerr << "    ERROR: texture index out of range\n";
            return;
        }

        const auto& tex = out.textures[textureIndex];

        std::cerr << "    texture name=" << tex.name << "\n";

        if (!tex.source)
        {
            std::cerr << "    image: none\n";
            return;
        }

        const auto imageIndex = *tex.source;

        std::cerr << "    image index=" << imageIndex << "\n";

        if (imageIndex >= out.images.size())
        {
            std::cerr << "    ERROR: image index out of range\n";
            return;
        }

        const auto& img = out.images[imageIndex];

        std::cerr << "    image name=" << img.name << "\n";
        std::cerr << "    image uri=" << img.uri << "\n";
        std::cerr << "    encoded size=" << img.encodedBytes.size() << "\n";
    }

    void DebugNormalTextureSlot(const ModelImportData& out, const char* label, const glType::ImportNormalTexture& info)
    {
        std::cerr << "  " << label << "\n";

        if (!info.index)
        {
            std::cerr << "    texture: none\n";
            return;
        }

        const auto textureIndex = *info.index;

        std::cerr << "    texture index=" << textureIndex << "\n";
        std::cerr << "    texCoord=" << info.texCoord << "\n";
        std::cerr << "    scale=" << info.scale << "\n";

        if (textureIndex >= out.textures.size())
        {
            std::cerr << "    ERROR: texture index out of range\n";
            return;
        }

        const auto& tex = out.textures[textureIndex];

        std::cerr << "    texture name=" << tex.name << "\n";

        if (!tex.source)
        {
            std::cerr << "    image: none\n";
            return;
        }

        const auto imageIndex = *tex.source;

        std::cerr << "    image index=" << imageIndex << "\n";

        if (imageIndex >= out.images.size())
        {
            std::cerr << "    ERROR: image index out of range\n";
            return;
        }

        const auto& img = out.images[imageIndex];

        std::cerr << "    image name=" << img.name << "\n";
        std::cerr << "    image uri=" << img.uri << "\n";
        std::cerr << "    encoded size=" << img.encodedBytes.size() << "\n";
    }

    void DebugOcclusionTextureSlot(const ModelImportData& out, const char* label,
                                   const glType::ImportOcclusionTexture& info)
    {
        std::cerr << "  " << label << "\n";

        if (!info.index)
        {
            std::cerr << "    texture: none\n";
            return;
        }

        const auto textureIndex = *info.index;

        std::cerr << "    texture index=" << textureIndex << "\n";
        std::cerr << "    texCoord=" << info.texCoord << "\n";
        std::cerr << "    strength=" << info.strength << "\n";

        if (textureIndex >= out.textures.size())
        {
            std::cerr << "    ERROR: texture index out of range\n";
            return;
        }

        const auto& tex = out.textures[textureIndex];

        std::cerr << "    texture name=" << tex.name << "\n";

        if (!tex.source)
        {
            std::cerr << "    image: none\n";
            return;
        }

        const auto imageIndex = *tex.source;

        std::cerr << "    image index=" << imageIndex << "\n";

        if (imageIndex >= out.images.size())
        {
            std::cerr << "    ERROR: image index out of range\n";
            return;
        }

        const auto& img = out.images[imageIndex];

        std::cerr << "    image name=" << img.name << "\n";
        std::cerr << "    image uri=" << img.uri << "\n";
        std::cerr << "    encoded size=" << img.encodedBytes.size() << "\n";
    }

    const char* ToString(::ddknd::graphics::types::AlphaMode mode)
    {
        switch (mode)
        {
        case ::ddknd::graphics::types::AlphaMode::OPAQUE:
            return "OPAQUE";
        case ::ddknd::graphics::types::AlphaMode::MASK:
            return "MASK";
        case ::ddknd::graphics::types::AlphaMode::BLEND:
            return "BLEND";
        default:
            return "UNKNOWN";
        }
    }

    void DebugImportedMaterials(const ModelImportData& out)
    {
        std::cerr << "================ Imported Materials Debug ================\n";
        std::cerr << "materials=" << out.materials.size() << "\n";
        std::cerr << "textures =" << out.textures.size() << "\n";
        std::cerr << "images   =" << out.images.size() << "\n";
        std::cerr << "samplers =" << out.samplers.size() << "\n";

        for (std::size_t mi = 0; mi < out.materials.size(); ++mi)
        {
            const auto& mat = out.materials[mi];
            const auto& pbr = mat.pbrMetallicRoughness;

            std::cerr << "----------------------------------------------------------\n";
            std::cerr << "material[" << mi << "] name=" << mat.name << "\n";

            std::cerr << "  baseColorFactor=(" << pbr.baseColorFactor.x() << ", " << pbr.baseColorFactor.y() << ", "
                      << pbr.baseColorFactor.z() << ", " << pbr.baseColorFactor.w() << ")\n";

            std::cerr << "  metallicFactor=" << pbr.metallicFactor << "\n";
            std::cerr << "  roughnessFactor=" << pbr.roughnessFactor << "\n";

            DebugTextureSlot(out, "baseColorTexture", pbr.baseColorTexture);

            DebugTextureSlot(out, "metallicRoughnessTexture", pbr.metallicRoughnessTexture);

            DebugNormalTextureSlot(out, "normalTexture", mat.normalTexture);

            DebugOcclusionTextureSlot(out, "occlusionTexture", mat.occlusionTexture);

            DebugTextureSlot(out, "emissiveTexture", mat.emissiveTexture);

            std::cerr << "  emissiveFactor=(" << mat.emissiveFactor.x() << ", " << mat.emissiveFactor.y() << ", "
                      << mat.emissiveFactor.z() << ")\n";

            std::cerr << "  alphaMode=" << ToString(mat.alphaMode) << "\n";
            std::cerr << "  alphaCutoff=" << mat.alphaCutoff << "\n";
            std::cerr << "  doubleSided=" << mat.doubleSided << "\n";
        }

        std::cerr << "==========================================================\n";
    }

    void debug(const cgltf_data* g, ModelImportData& out)
    {
        std::cerr << "material size=" << g->materials_count << "\n";
        std::cerr << "texture count=" << g->textures_count << "\n";
        std::cerr << "image count =" << g->images_count << "\n";

        for (cgltf_size mi = 0; mi < g->materials_count; mi++)
        {
            const cgltf_material& m = g->materials[mi];
            const auto& pbr = m.pbr_metallic_roughness;

            if (pbr.base_color_texture.texture)
            {
                const cgltf_texture* tex = pbr.base_color_texture.texture;
                const cgltf_image* img = tex->image;

                std::cerr << "material[" << mi << "] name=" << (m.name ? m.name : "") << "\n";
                std::cerr << " has baseColorTexture\n";

                if (img)
                {
                    std::cerr << " image name=" << (img->name ? img->name : "") << "\n";
                    std::cerr << " image uri=" << (img->uri ? img->uri : "") << "\n";
                    std::cerr << "image mime=" << (img->mime_type ? img->mime_type : "(null)") << "\n";
                    std::cerr << "has buffer_view=" << (img->buffer_view != nullptr) << "\n";
                }
            }
        }
    }

    void DebugImportedMaterialLinks(const ModelImportData& out)
    {
        std::cerr << "materials=" << out.materials.size() << "\n";
        std::cerr << "textures =" << out.textures.size() << "\n";
        std::cerr << "images   =" << out.images.size() << "\n";

        for (std::size_t mi = 0; mi < out.materials.size(); mi++)
        {
            const auto& mat = out.materials[mi];

            std::cerr << "material[" << mi << "] name=" << mat.name << "\n";
            const auto& baseColor = mat.pbrMetallicRoughness.baseColorTexture;

            if (!baseColor.index)
            {
                std::cerr << "      baseColorTexture: none\n";
                continue;
            }

            const auto textureIndex = *baseColor.index;
            std::cerr << "      baseColorTexture texture index=" << textureIndex << "\n";

            if (textureIndex >= out.textures.size())
            {
                std::cerr << "      ERROR: texture index out of range\n";
                continue;
            }

            const auto& tex = out.textures[textureIndex];
            if (!tex.source)
            {
                std::cerr << "      texture source image: none\n";
                continue;
            }

            const auto& imageIndex = *tex.source;
            
            if (imageIndex >= out.images.size())
            {
                std::cerr << "      ERROR: image index out of range\n";
                continue;
            }

            const auto& img = out.images[imageIndex];

            std::cerr << "      image name=" << img.name << "\n";
            std::cerr << "      image uri=" << img.uri << "\n";
            std::cerr << "      image encoded size=" << img.encodedBytes.size() << "\n";
        }
    }
}