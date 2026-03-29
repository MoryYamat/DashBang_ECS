#include "internal/asset/resource_loader.h"

#include "internal/asset/mesh_text_parser.h"
#include "internal/asset/shader_descriptor_parser.h"
#include "internal/io/io.h"

#include "io/io.h"

#include <spdlog/spdlog.h>

namespace ddknd::asset::loader
{

    std::unique_ptr<ddknd::asset::type::ShaderResource> ResourceLoader<ddknd::asset::type::ShaderResource>::Load(
        const ddknd::io::IPathResolver& vfs, ddknd::graphics::IRendererBackend& backend, std::string_view vpath)
    {
        auto abs = vfs.TryResolve(vpath);
        if (!abs)
        {
            spdlog::error("ShaderLoader: resolve shader desc failed: {}", std::string(vpath));
            return nullptr;
        }

        auto textOpt = ::ddknd::io::ReadAllText(*abs);
        if (!textOpt)
        {
            spdlog::error("ShaderLoader: ReadAllText failed: {}", abs->string());
            return nullptr;
        }

        ddknd::asset::parser::ParseError err;
        auto docOpt = ParseKeyValueText(*textOpt, &err);
        if (!docOpt)
        {
            spdlog::error("ShaderLoader: ParseKeyValueText failed line={} msg={}", err.line, err.msg);
            return nullptr;
        }

        auto descOpt = ::ddknd::asset::parser::BuildShaderDesc(*docOpt, &err);
        if (!descOpt)
        {
            spdlog::error("ShaderLoader: BuildShaderDesc failed line={} msg={}", err.line, err.msg);
            return nullptr;
        }

        spdlog::info("ShaderLoader: desc vertex={} fragment={}", descOpt->vertex_path, descOpt->fragment_path);

        auto vsAbs = vfs.TryResolve(descOpt->vertex_path);
        if (!vsAbs)
        {
            spdlog::error("ShaderLoader: resolve vertex failed: {}", descOpt->vertex_path);
            return nullptr;
        }

        auto fsAbs = vfs.TryResolve(descOpt->fragment_path);
        if (!fsAbs)
        {
            spdlog::error("ShaderLoader: resolve fragment failed: {}", descOpt->fragment_path);
            return nullptr;
        }

        spdlog::info("ShaderLoader: desc abs path = {}", abs->string());
        spdlog::info("ShaderLoader: vs abs path = {}", vsAbs->string());
        spdlog::info("ShaderLoader: fs abs path = {}", fsAbs->string());

        auto vsText = ::ddknd::io::ReadAllText(*vsAbs);
        if (!vsText)
        {
            spdlog::error("ShaderLoader: ReadAllText vertex failed: {}", vsAbs->string());
            return nullptr;
        }

        auto fsText = ::ddknd::io::ReadAllText(*fsAbs);
        if (!fsText)
        {
            spdlog::error("ShaderLoader: ReadAllText fragment failed: {}", fsAbs->string());
            return nullptr;
        }

        auto prog = backend.CreateShaderProgram(*vsText, *fsText);
        if (!prog.Is_valid())
        {
            spdlog::error("ShaderLoader: CreateShaderProgram failed");
            return nullptr;
        }

        auto r = std::make_unique<asset::type::ShaderResource>();
        r->program = prog;
        return r; // move (unique_ptr)
    }


    std::unique_ptr<ddknd::asset::type::MeshResource> ResourceLoader<ddknd::asset::type::MeshResource>::Load(const ddknd::io::IPathResolver& vfs,
                                                         ddknd::graphics::IRendererBackend& backend,
                                                         std::string_view vpath)
    {

        auto abs = vfs.TryResolve(vpath);
        if (!abs)
        {
            spdlog::error("MeshLoader::Load failed to resolve virtual path");
            return nullptr;
        }

        // read file (今は未使用)
        const auto textOpt = ::ddknd::io::ReadAllText(*abs);
        if (!textOpt)
        {
            spdlog::error("MeshLoader::Load: failed to read file: {}", abs->string());
            return nullptr;
        }

        auto parsed = ::ddknd::asset::parser::ParseMeshPos3Text(*textOpt);
        if (!parsed)
        {
            spdlog::error("MeshLoader::Load: parse failed: {}", abs->string());
            return nullptr;
        }

        if ((parsed->xyz.size() % 3) != 0)
        {
            spdlog::error("MeshLoader::Load: invalid xyz size: {}", parsed->xyz.size());
            return nullptr;
        }

        const std::uint32_t vertexCount = static_cast<std::uint32_t>(parsed->xyz.size() / 3);

        //// test rectagle
        // const float verts[] =
        //{
        //    -0.5f, -0.5f, 0.0f,
        //	0.5f, -0.5f, 0.0f,
        //	0.0f,  0.5f, 0.0f
        // };

        auto meshId = backend.CreateMesh_Pos3(std::span<const float>(parsed->xyz.data(), parsed->xyz.size()));
        if (!meshId.Is_valid())
        {
            spdlog::error("MeshLoader::Load: invalid MeshID");
            return nullptr;
        }

        auto r = std::make_unique<::ddknd::asset::type::MeshResource>();
        r->mesh = meshId;
        r->vertexCount = 3;

        return r; // move (unique_ptr)
    }
} // namespace ddknd::asset::loader