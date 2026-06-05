#pragma once

#include "ddknd/asset/asset_fwd.h"
#include "ddknd/asset/asset_manager.h"
#include "ddknd/asset/asset_tag.h"
#include "gfx_type.h"
#include "ddknd/graphics/graphics_fwd.h"
#include "ddknd/graphics/model_data.h"
#include "ddknd/graphics/renderer.h"
#include "ddknd/io/io.h"
#include "ddknd/io/io_fwd.h"


namespace ddknd::animation
{
    class AnimationAssetStore
    {
      public:
        using AnimationTag = ::ddknd::animation::tag::AnimationClipTag;
        using AnimationID = ::ddknd::asset::AssetID<::ddknd::animation::tag::AnimationClipTag>;
        using AnimationClipResource = ::ddknd::animation::types::AnimationClipResource;

        const AnimationClipResource* TryGet(AnimationID id) const
        {
          return anims_.TryGet(id);
        }
        void SetLoaded(AnimationID id, AnimationClipResource res)
        {
          anims_.Set(id, std::move(res));
        }

      private:
        template <typename T, typename Tag>
        using AssestStorage = ::ddknd::asset::AssestStorage<T, Tag>;

        AssestStorage<AnimationClipResource, AnimationTag> anims_;
    };

} // namespace ddknd::animation

namespace ddknd::graphics
{
    class GraphicsAssetStore
    {
      public:
        template <typename Tag>
        using AssetID = ::ddknd::asset::AssetID<Tag>;

        using ShaderTag = ::ddknd::asset::tag::Shader;
        using ModelTag = ::ddknd::asset::tag::Model; //@TODO Model {render, skeleton, clip}
        using FontTag = ::ddknd::asset::tag::Font;

        using ShaderID = AssetID<ShaderTag>;
        using ModelID = AssetID<ModelTag>;
        using FontID = AssetID<FontTag>;

        using ShaderResource = asset::ShaderResource;
        using ModelRenderResource = asset::ModelRenderResource;
        using FontResource = asset::FontResource;

        const ShaderResource* TryGet(ShaderID id) const
        {
            return shaders_.TryGet(id);
        }
        const ModelRenderResource* TryGet(ModelID id) const
        {
            return models_.TryGet(id);
        }
        const FontResource* TryGet(FontID id) const
        {
          return fonts_.TryGet(id);
        }

        void SetLoaded(ShaderID id, ShaderResource res)
        {
            shaders_.Set(id, std::move(res));
        }
        void SetLoaded(ModelID id, ModelRenderResource res)
        {
            models_.Set(id, std::move(res));
        }
        void SetLoaded(FontID id, FontResource res)
        {
            fonts_.Set(id, std::move(res));
        }

      private:
        template <typename T, typename Tag>
        using AssetStorage = ::ddknd::asset::AssestStorage<T, Tag>;

        AssetStorage<ShaderResource, ShaderTag> shaders_;
        AssetStorage<ModelRenderResource, ModelTag> models_;
        AssetStorage<FontResource, FontTag> fonts_;
    };

    class GraphicsAssetLoader
    {
        using AssetManager = ::ddknd::asset::AssetManager;
        template <typename Tag>
        using AssetID = ::ddknd::asset::AssetID<Tag>;

        using ShaderTag = ::ddknd::asset::tag::Shader;
        using ModelTag = ::ddknd::asset::tag::Model;
        using FontTag = ::ddknd::asset::tag::Font;

        using ShaderID = AssetID<ShaderTag>;
        using ModelID = AssetID<ModelTag>;
        using FontID = AssetID<FontTag>;
        
        using ModelRenderResource = asset::ModelRenderResource;

        using IPathResolver = ::ddknd::io::IPathResolver;
        using IRendererBackend = ::ddknd::graphics::IRendererBackend;

      public:
        using AnimationAssetStore = ::ddknd::animation::AnimationAssetStore;

        GraphicsAssetLoader(const IPathResolver& resolver, IRendererBackend& backend)
            : resolver_(resolver), backend_(backend)
        {
        }

        bool LoadShader(AssetManager& assets, GraphicsAssetStore& store, ShaderID id);
        bool LoadModel(AssetManager& assets, GraphicsAssetStore& gfxstore, AnimationAssetStore& animstore, ModelID id);
        bool LoadFont(AssetManager& assets, GraphicsAssetStore& store, FontID id);
      private:
        const IPathResolver& resolver_;
        IRendererBackend& backend_;
    };
} // namespace ddknd::graphics
