#pragma once

#include "ddknd/asset/asset_fwd.h"
#include "ddknd/asset/asset_manager.h"
#include "ddknd/asset/asset_tag.h"
#include "ddknd/graphics/graphics_fwd.h"
// #include "ddknd/graphics/model_data.h"
#include "ddknd/graphics/renderer.h"
#include "ddknd/io/io.h"
#include "ddknd/io/io_fwd.h"
#include "gfx_type.h"

namespace ddknd::animation
{
    class AnimationAssetStore
    {
      private:
        using AnimationClipTag = ::ddknd::asset::tag::AnimationClip;
        using AnimationID = ::ddknd::asset::AssetID<AnimationClipTag>;
        using AnimationClipResource = ::ddknd::animation::types::AnimationClipResource;

      public:
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
        using AssestStorage = ::ddknd::asset::AssetStorage<T, Tag>;

        AssestStorage<AnimationClipResource, AnimationClipTag> anims_;
    };

} // namespace ddknd::animation

namespace ddknd::graphics
{
    class GraphicsAssetStore
    {
      private:
        template <typename Tag>
        using AssetID = ::ddknd::asset::AssetID<Tag>;

        using ShaderTag = ::ddknd::asset::tag::Shader;
        using ModelTag = ::ddknd::asset::tag::Model; //@TODO Model {render, skeleton, clip}
        using FontTag = ::ddknd::asset::tag::Font;
        using TextureTag = ::ddknd::asset::tag::TextureTag;

        using ShaderID = AssetID<ShaderTag>;
        using ModelID = AssetID<ModelTag>;
        using FontID = AssetID<FontTag>;
        using TextureID = AssetID<TextureTag>;

        using ShaderResource = ::ddknd::graphics::types::ShaderResource;
        using ModelRenderResource = ::ddknd::graphics::types::ModelRenderResource;
        using FontResource = ::ddknd::graphics::types::FontResource;
        using TextureResource = ::ddknd::graphics::types::TextureResource;

      public:
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
        const TextureResource* TryGet(TextureID id) const
        {
            return textures_.TryGet(id);
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
        void SetLoaded(TextureID id, TextureResource res)
        {
            textures_.Set(id, std::move(res));
        }

      private:
        template <typename T, typename Tag>
        using AssetStorage = ::ddknd::asset::AssetStorage<T, Tag>;

        AssetStorage<ShaderResource, ShaderTag> shaders_;
        AssetStorage<ModelRenderResource, ModelTag> models_;
        AssetStorage<FontResource, FontTag> fonts_;
        AssetStorage<TextureResource, TextureTag> textures_;
    };

    class GraphicsAssetLoader
    {
      private:
        using AssetManager = ::ddknd::asset::AssetManager;
        template <typename Tag>
        using AssetID = ::ddknd::asset::AssetID<Tag>;

        using ShaderTag = ::ddknd::asset::tag::Shader;
        using ModelTag = ::ddknd::asset::tag::Model;
        using FontTag = ::ddknd::asset::tag::Font;
        using AnimTag = ::ddknd::asset::tag::AnimationClip;//
        using TextureTag = ::ddknd::asset::tag::TextureTag;

        using ShaderID = AssetID<ShaderTag>;
        using ModelID = AssetID<ModelTag>;
        using FontID = AssetID<FontTag>;
        using TextureID = AssetID<TextureTag>;

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
        bool LoadTexture(AssetManager& assets, GraphicsAssetStore& store, TextureID id);

      private:
        const IPathResolver& resolver_;
        IRendererBackend& backend_;
    };
} // namespace ddknd::graphics
