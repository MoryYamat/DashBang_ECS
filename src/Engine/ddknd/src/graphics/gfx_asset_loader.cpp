#include "graphics/gfx_asset_loader.h"

#include "asset/asset_manager.h"

#include "internal/io/io.h"
#include "internal/asset/shader_descriptor_parser.h"

#include "internal/graphics/model_importer/glb_importer.h"

#include <string_view>
#include <spdlog/spdlog.h>

namespace ddknd::graphics
{
    bool GraphicsAssetLoader::LoadShader(AssetManager& assets, GraphicsAssetStore& store, ShaderID id)
    {
        auto vpath = assets.TryPathOf<ShaderID::tag_type>(id);
        if(!vpath)
            return false;

        auto abs = resolver_.TryResolve(*vpath);
        if(!abs)
        {
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        using namespace ::ddknd::io;
        auto textOpt = ReadAllText(*abs);
        if (!textOpt)
        {
            spdlog::error("ShaderLoader: ReadAllText failed: {}", abs->string());
            return false;
        }

        using namespace ::ddknd::asset::parser;
        ParseError err;
        auto docOpt = ParseKeyValueText(*textOpt, &err);
        if (!docOpt)
        {
            spdlog::error("ShaderLoader: ParseKeyValueText failed line={} msg={}", err.line, err.msg);
            return false;
        }

        auto descOpt = BuildShaderDesc(*docOpt, &err);
        if (!descOpt)
        {
            spdlog::error("ShaderLoader: BuildShaderDesc failed line={} msg={}", err.line, err.msg);
            return false;
        }

                spdlog::info("ShaderLoader: desc vertex={} fragment={}", descOpt->vertex_path, descOpt->fragment_path);

        auto vsAbs = resolver_.TryResolve(descOpt->vertex_path);
        if (!vsAbs)
        {
            spdlog::error("ShaderLoader: resolve vertex failed: {}", descOpt->vertex_path);
            return false;
        }

        auto fsAbs = resolver_.TryResolve(descOpt->fragment_path);
        if (!fsAbs)
        {
            spdlog::error("ShaderLoader: resolve fragment failed: {}", descOpt->fragment_path);
            return false;
        }

        spdlog::info("ShaderLoader: desc abs path = {}", abs->string());
        spdlog::info("ShaderLoader: vs abs path = {}", vsAbs->string());
        spdlog::info("ShaderLoader: fs abs path = {}", fsAbs->string());

        auto vsText = ::ddknd::io::ReadAllText(*vsAbs);
        if (!vsText)
        {
            spdlog::error("ShaderLoader: ReadAllText vertex failed: {}", vsAbs->string());
            return false;
        }

        auto fsText = ::ddknd::io::ReadAllText(*fsAbs);
        if (!fsText)
        {
            spdlog::error("ShaderLoader: ReadAllText fragment failed: {}", fsAbs->string());
            return false;
        }

        auto prog = backend_.CreateShaderProgram(*vsText, *fsText);
        if (!prog.Is_valid())
        {
            spdlog::error("ShaderLoader: CreateShaderProgram failed");
            return false;
        }

        asset::ShaderResource res{};
        res.program  = prog;

        store.SetLoaded(id, std::move(res));
        assets.SetState(id, ::ddknd::asset::AssetState::Loaded);

        return true;
    }

    //
    bool GraphicsAssetLoader::LoadModel(AssetManager& assets, GraphicsAssetStore& gfxstore, AnimationAssetStore& animstore, ModelID id)
    {
        auto vpath = assets.TryPathOf<ModelID::tag_type>(id);
        if(!vpath)
            return false;

        auto path = resolver_.TryResolve(*vpath);
        if(!path)
        {
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        const std::string abs = path->string();

        // importer
        auto imported = ddknd::graphics::internal::ImportModel(abs);

        // build and set resources
        // gfxstore.SetLoaded(id, BuildRenderResource(imported));
        // animstore.SetLoaded(id, BUildAnimationResource(imported));

        return true;
    }


} // namespace ddknd::graphics