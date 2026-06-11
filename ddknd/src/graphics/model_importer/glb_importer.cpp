#include "internal/graphics/model_importer/glb_importer.h"
#include "internal/graphics/model_importer/model_import_types.h"

#include <algorithm>
#include <limits>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include <spdlog/spdlog.h>

#include "ddknd/math/math.h"

// document
// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#foreword

// ==================================== alias ====================================
namespace glType = ::ddknd::graphics::internal::types;
using ModelImportData = glType::ModelImportData;
using ImportNode = glType::ImportNode;
using ImportPrimitive = glType::ImportPrimitive;
using Vertex = glType::Vertex;
using ImportChannel = glType::ImportChannel;
using ChannelType = glType::ChannelType;

namespace math = ::ddknd::math;
using Vec2f = math::Vec2f;
using Vec3f = math::Vec3f;
using Vec4f = math::Vec4f;
using uVec4 = math::uVec4;
using Quatf = math::Quatf;
// ===============================================================================

// helper
namespace
{
    using Vec3f = ::ddknd::math::Vec3f;
    using Quat = ::ddknd::math::Quatf;
    using Mat4f = ::ddknd::math::Mat4f;

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

    Mat4f ReadNodeLocalMatrix(const cgltf_node* n)
    {
        using namespace ddknd::math;

        if (n->has_matrix)
        {
            // 注意: glTF の matrix は column-major。
            // 自前 Mat が row-major なら転置読み込み
            Mat4f m{};
            for (std::size_t r = 0; r < 4; ++r)
            {
                for (std::size_t c = 0; c < 4; ++c)
                {
                    m(r, c) = n->matrix[c * 4 + r];
                }
            }
            return m;
        }

        Vec3f T = n->has_translation ? Vec3f{n->translation[0], n->translation[1], n->translation[2]} : Vec3f{0, 0, 0};

        Quatf R =
            n->has_rotation ? Quatf{n->rotation[3], n->rotation[0], n->rotation[1], n->rotation[2]} : Quatf::Identity();

        Vec3f S = n->has_scale ? Vec3f{n->scale[0], n->scale[1], n->scale[2]} : Vec3f{1, 1, 1};

        return translate(T) * mat4_cast(R) * scale(S);
    }

    math::TRS ReadNodeLocalTRS(const cgltf_node* n)
    {
        math::TRS trs{};

        if (n->has_translation)
            trs.translation = {n->translation[0], n->translation[1], n->translation[2]};

        if (n->has_rotation)
            trs.rotation = {n->rotation[3], n->rotation[0], n->rotation[1], n->rotation[2]};

        if (n->has_scale)
            trs.scale = {n->scale[0], n->scale[1], n->scale[2]};

        return trs;
    }

    void BuildNodeGlobals(const cgltf_data* g, std::vector<int>& parent, std::vector<Mat4f>& globals)
    {
        globals.assign(g->nodes_count, Mat4f::Identity());
        std::function<void(int)> dfs = [&](int idx)
        {
            const cgltf_node* n = &g->nodes[idx];
            int p = parent[idx];
            globals[idx] = (p >= 0 ? globals[p] : Mat4f::Identity()) * ReadNodeLocalMatrix(n);
            for (cgltf_size ci = 0; ci < n->children_count; ++ci)
            {
                int child_index = int(n->children[ci] - g->nodes);
                dfs(child_index);
            }
        };

        for (cgltf_size ni = 0; ni < g->nodes_count; ++ni)
        {
            if (parent[ni] < 0)
                dfs(int(ni));
        }
    }

    template <class T>
    int IndexOf(const T* base, std::size_t count, const T* ptr)
    {
        if (!ptr)
            return -1;

        for (std::size_t i = 0; i < count; ++i)
        {
            if (&base[i] == ptr)
            {
                if (i > static_cast<std::size_t>(std::numeric_limits<int>::max()))
                    return -1;

                return static_cast<int>(i);
            }
        }

        return -1;
    }

    // =================== helpers ===================
    std::vector<Vec2f> ReadVec2(const cgltf_accessor* acc)
    {
        std::vector<Vec2f> out(acc->count);

        float tmp[2] = {};
        for (cgltf_size i = 0; i < acc->count; i++)
        {
            cgltf_accessor_read_float(acc, i, tmp, 2);
            out[i] = Vec2f{tmp[0], tmp[1]};
        }
        return out;
    }

    std::vector<Vec3f> ReadVec3(const cgltf_accessor* acc)
    {
        std::vector<Vec3f> out(acc->count);

        float tmp[3] = {};
        for (cgltf_size i = 0; i < acc->count; i++)
        {
            cgltf_accessor_read_float(acc, i, tmp, 3);
            out[i] = Vec3f{tmp[0], tmp[1], tmp[2]};
        }
        return out;
    }

    std::vector<Vec4f> ReadVec4(const cgltf_accessor* acc)
    {
        std::vector<Vec4f> out(acc->count);

        float tmp[4] = {};
        for (cgltf_size i = 0; i < acc->count; i++)
        {
            cgltf_accessor_read_float(acc, i, tmp, 4);
            out[i] = Vec4f{tmp[0], tmp[1], tmp[2], tmp[3]};
        }
        return out;
    }

    std::vector<uVec4> ReadUVec4(const cgltf_accessor* acc)
    {
        std::vector<uVec4> out(acc->count);

        std::uint32_t tmp[4] = {};
        for (cgltf_size i = 0; i < acc->count; i++)
        {
            cgltf_accessor_read_uint(acc, i, tmp, 4);
            out[i] = uVec4{tmp[0], tmp[1], tmp[2], tmp[3]};
        }
        return out;
    }

    std::vector<std::uint32_t> ReadIndices(const cgltf_accessor* acc)
    {
        std::vector<std::uint32_t> out(acc->count);

        for (cgltf_size i = 0; i < acc->count; i++)
        {
            const cgltf_size index = cgltf_accessor_read_index(acc, i);
            out[i] = static_cast<std::uint32_t>(index);
        }

        return out;
    }

} // namespace

// functions
namespace
{
    void ReadNodes(const cgltf_data* g, ModelImportData& out)
    {
        out.nodes.resize(g->nodes_count);

        for (cgltf_size ni = 0; ni < g->nodes_count; ni++)
        {
            const cgltf_node* src = &g->nodes[ni];
            auto& dst = out.nodes[ni];

            dst.name = src->name ? src->name : "";

            dst.parent = -1;
            if (src->mesh)
                dst.mesh = IndexOf(g->meshes, g->meshes_count, src->mesh);

            if (src->skin)
                dst.skin = IndexOf(g->skins, g->skins_count, src->skin);

            dst.localMatrix = ReadNodeLocalMatrix(src);
            dst.localTRS = ReadNodeLocalTRS(src);
            // std::cerr << dst.name << "\n";
            // std::cerr << " ======== local matrix ======= \n" << dst.localMatrix << "\n";
        }

        for (cgltf_size ni = 0; ni < g->nodes_count; ni++)
        {
            const cgltf_node* src = &g->nodes[ni];
            auto& dst = out.nodes[ni];

            dst.children.reserve(src->children_count);

            for (cgltf_size ci = 0; ci < src->children_count; ci++)
            {
                const cgltf_node* child = src->children[ci];

                int childIndex = IndexOf(g->nodes, g->nodes_count, child);
                if (childIndex < 0)
                {
                    continue;
                }

                dst.children.push_back(childIndex);
                out.nodes[childIndex].parent = static_cast<int>(ni);

                // std::cerr << "child index=" << childIndex << "\n";
            }
        }
    }

    void ReadScenes(const cgltf_data* g, ModelImportData& out)
    {
        out.scenes.resize(g->scenes_count);

        for (cgltf_size si = 0; si < g->scenes_count; si++)
        {
            const cgltf_scene* src = &g->scenes[si];
            auto& dst = out.scenes[si];

            dst.name = src->name ? src->name : "";
            // std::cerr << "scene_name=" << dst.name << "\n";
            dst.rootNodes.reserve(src->nodes_count);
            for (cgltf_size ni = 0; ni < src->nodes_count; ni++)
            {
                const cgltf_node* root = src->nodes[ni];

                int nodeIndex = IndexOf(g->nodes, g->nodes_count, root);
                if (nodeIndex < 0)
                    continue;

                dst.rootNodes.push_back(nodeIndex);
            }
        }

        // default scene
        if (g->scene)
        {
            out.defaultScene = IndexOf(g->scenes, g->scenes_count, g->scene);
        }
        else
        {
            out.defaultScene = g->scenes_count > 0 ? 0 : -1;
        }
    }

    int ReadPrimitives(const cgltf_data* g, const cgltf_primitive& srcPrim, ModelImportData& out)
    {
        ImportPrimitive dst;

        const cgltf_accessor* acc_pos = nullptr;
        const cgltf_accessor* acc_normal = nullptr;
        const cgltf_accessor* acc_uv = nullptr;
        const cgltf_accessor* acc_tangent = nullptr;
        const cgltf_accessor* acc_joints = nullptr;
        const cgltf_accessor* acc_weights = nullptr;

        for (cgltf_size ai = 0; ai < srcPrim.attributes_count; ai++)
        {
            const cgltf_attribute& a = srcPrim.attributes[ai];

            switch (a.type)
            {
            case cgltf_attribute_type_position:
                acc_pos = a.data;
                break;
            case cgltf_attribute_type_normal:
                acc_normal = a.data;
                break;
            case cgltf_attribute_type_texcoord:
                if (a.index == 0)
                    acc_uv = a.data;
                break;
            case cgltf_attribute_type_tangent:
                acc_tangent = a.data;
                break;
            case cgltf_attribute_type_joints:
                acc_joints = a.data;
                break;
            case cgltf_attribute_type_weights:
                acc_weights = a.data;
                break;
            default:
                break;
            }
        }

        if (!acc_pos)
            return -1;
        auto positions = ReadVec3(acc_pos);
        auto normals = acc_normal ? ReadVec3(acc_normal) : std::vector<Vec3f>{};
        auto uvs = acc_uv ? ReadVec2(acc_uv) : std::vector<Vec2f>{};
        auto tangents = acc_tangent ? ReadVec4(acc_tangent) : std::vector<Vec4f>{};
        auto joints = acc_joints ? ReadUVec4(acc_joints) : std::vector<uVec4>{};
        auto weights = acc_weights ? ReadVec4(acc_weights) : std::vector<Vec4f>{};

        std::size_t vcount = positions.size();
        dst.vertices.resize(vcount);

        for (std::size_t i = 0; i < vcount; i++)
        {
            Vertex v{};
            v.pos = positions[i];

            if (!normals.empty())
                v.normal = normals[i];
            if (!uvs.empty())
                v.texCoords = uvs[i];
            if (!tangents.empty())
                v.tangent = tangents[i];
            if (!joints.empty())
                v.joints = joints[i];
            if (!weights.empty())
                v.weights = weights[i];

            dst.vertices[i] = v;
        }

        if (srcPrim.indices)
        {
            dst.indices = ReadIndices(srcPrim.indices);
        }
        else
        {
            dst.indices.resize(vcount);
            for (std::uint32_t i = 0; i < vcount; i++)
            {
                dst.indices[i] = i;
            }
        }

        if (srcPrim.material)
        {
            dst.material = IndexOf(g->materials, g->materials_count, srcPrim.material);
        }

        int index = static_cast<int>(out.primitives.size());
        out.primitives.push_back(std::move(dst));

        return index;
    }

    void ReadSkins(const cgltf_data* g, ModelImportData& out)
    {
        out.skins.resize(g->skins_count);
        for (cgltf_size si = 0; si < g->skins_count; si++)
        {
            const cgltf_skin* src = &g->skins[si];
            auto& dst = out.skins[si];

            dst.name = src->name ? src->name : "";

            dst.skeletonRootNode = src->skeleton ? IndexOf(g->nodes, g->nodes_count, src->skeleton) : -1;

            dst.jointNodes.resize(src->joints_count);
            for (cgltf_size ji = 0; ji < src->joints_count; ji++)
            {
                const cgltf_node* jn = src->joints[ji];

                dst.jointNodes[ji] = IndexOf(g->nodes, g->nodes_count, jn);
            }

            dst.inverseBindMatrices.assign(src->joints_count, Mat4f::Identity());

            if (src->inverse_bind_matrices)
            {
                const cgltf_accessor* acc = src->inverse_bind_matrices;
                const cgltf_size count = std::min(acc->count, src->joints_count);

                float m[16];

                for (cgltf_size i = 0; i < count; i++)
                {
                    cgltf_accessor_read_float(acc, i, m, 16);

                    Mat4f mat{};
                    for (int r = 0; r < 4; r++)
                    {
                        for (int c = 0; c < 4; c++)
                        {
                            mat(r, c) = m[c * 4 + r]; //  column -> row
                        }
                    }

                    dst.inverseBindMatrices[i] = mat;
                }
            }
        }
    }

    void ReadMeshes(const cgltf_data* g, ModelImportData& out)
    {
        out.meshes.resize(g->meshes_count);
        for (cgltf_size mi = 0; mi < g->meshes_count; mi++)
        {
            const cgltf_mesh* srcMesh = &g->meshes[mi];
            auto& dstMesh = out.meshes[mi];

            dstMesh.name = srcMesh->name ? srcMesh->name : "";

            for (cgltf_size pi = 0; pi < srcMesh->primitives_count; pi++)
            {
                const cgltf_primitive& srcPrim = srcMesh->primitives[pi];

                // read: srcPrim.attributes / srcPrim.indices / srcPrim.material
                int primitiveIndex = ReadPrimitives(g, srcPrim, out);
                if (primitiveIndex >= 0)
                    dstMesh.primitives.push_back(primitiveIndex);
            }
        }
    }

    void ReadAnimations(const cgltf_data* g, ModelImportData& out)
    {
        out.animations.resize(g->animations_count);

        for (cgltf_size ai = 0; ai < g->animations_count; ai++)
        {
            const cgltf_animation* srcAnim = &g->animations[ai];
            auto& dstAnim = out.animations[ai];

            dstAnim.name = srcAnim->name ? srcAnim->name : "anim_" + std::to_string(ai);
            std::cerr << "anim_name=" << dstAnim.name << "\n";

            for (cgltf_size ci = 0; ci < srcAnim->channels_count; ci++)
            {
                const cgltf_animation_channel& srcCh = srcAnim->channels[ci];

                if (!srcCh.target_node || !srcCh.sampler)
                    continue;

                ImportChannel ch{};
                ch.targetNode = IndexOf(g->nodes, g->nodes_count, srcCh.target_node); // nodeIdx

                if (ch.targetNode < 0)
                    continue;

                switch (srcCh.target_path)
                {
                case cgltf_animation_path_type_translation:
                    ch.type = ChannelType::T;
                    break;
                case cgltf_animation_path_type_rotation:
                    ch.type = ChannelType::R;
                    break;
                case cgltf_animation_path_type_scale:
                    ch.type = ChannelType::S;
                    break;
                default:
                    continue; // ignore weights/morph target
                }

                const cgltf_animation_sampler& sampler = *srcCh.sampler;

                if (!sampler.input || !sampler.output)
                    continue;

                ch.times.resize(sampler.input->count);
                for (cgltf_size i = 0; i < sampler.input->count; i++)
                {
                    float t = 0.0f;
                    cgltf_accessor_read_float(sampler.input, i, &t, 1);
                    ch.times[i] = t;
                    dstAnim.duration = std::max(dstAnim.duration, t);
                }

                if (ch.type == ChannelType::T || ch.type == ChannelType::S)
                {
                    ch.v3 = ReadVec3(sampler.output);
                }
                else if (ch.type == ChannelType::R)
                {
                    ch.vq.resize(sampler.output->count);
                    float q[4] = {};
                    for (cgltf_size i = 0; i < sampler.output->count; i++)
                    {
                        cgltf_accessor_read_float(sampler.output, i, q, 4);

                        ch.vq[i] = Quatf{q[3], q[0], q[1], q[2]}; // myQuat = (w, x, y, z)
                        ch.vq[i].Normalize();
                    }
                }

                dstAnim.channels.push_back(std::move(ch));
            }
        }
    }

} // namespace

namespace ddknd::graphics::internal
{
    // The importer does not consider the runtime data structure.
    std::optional<::ddknd::graphics::internal::types::ModelImportData> ImportModel(const std::string& path)
    {

        ModelImportData model{};

        cgltf_options options{};
        cgltf_data* g = nullptr; // accessors

        std::cerr << "model path = " << path << "\n";
        if (cgltf_parse_file(&options, path.c_str(), &g) != cgltf_result_success || !g)
        {
            spdlog::warn("[CgltfImporter]: Parse information creation failure\n");
            return std::nullopt;
        }

        if (cgltf_load_buffers(&options, g, path.c_str()) != cgltf_result_success)
        {
            spdlog::warn("[CgltfImporter]: Failed to read data from file\n");
            cgltf_free(g);
            return std::nullopt;
        }

        if (cgltf_validate(g) != cgltf_result_success)
        {
            spdlog::warn("[CgltfImporter]: Invalid glTF");
            cgltf_free(g);
            return std::nullopt;
        }

        ReadNodes(g, model);
        ReadScenes(g, model);
        ReadMeshes(g, model);
        ReadSkins(g, model);
        ReadAnimations(g, model);

        cgltf_free(g);
        return model;
    }
} // namespace ddknd::graphics::internal

// cglft specifications
// https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#foreword

// 7. skinを読む
// 8. animationを読む