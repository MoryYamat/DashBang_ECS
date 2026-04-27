#pragma once

#include "asset/asset_fwd.h"
#include "asset/asset_manager.h"
#include "asset/asset_tag.h"
#include "gfx_type.h"
#include "internal/graphics/animation_data.h"

namespace ddknd::animation
{
    class AnimationAssetStore
    {
      public:
        using ModelID = ddknd::asset::AssetID<ddknd::asset::tag::Model>;
        using ModelAnimationResource = animation::asset::ModelAnimationResource;
        using ModelTag = ::ddknd::asset::tag::Model; //@TODO Model {render, skeleton, clip}

        const ModelAnimationResource* TryGet(ModelID id) const;
        void SetLoaded(ModelID id, ModelAnimationResource res);

      private:
        template <typename T, typename Tag>
        using Storage = ::ddknd::asset::Storage<T, Tag>;

        ddknd::asset::Storage<ModelAnimationResource, ModelTag> models_;
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

        using ShaderID = AssetID<ShaderTag>;
        using ModelID = AssetID<ModelTag>;
        using ShaderResource = asset::ShaderResource;
        using ModelResource = asset::ModelRenderResource;

        const ShaderResource* TryGet(ShaderID id) const;
        const ModelResource* TryGet(ModelID id) const;

        void SetLoaded(ShaderID id, ShaderResource res);
        void SetLoaded(ModelID id, ModelResource res);

      private:
        template <typename T, typename Tag>
        using Storage = ::ddknd::asset::Storage<T, Tag>;

        Storage<ShaderResource, ShaderTag> shaders_;
        Storage<ModelResource, ModelTag> models_;
    };

    class GraphicsAssetLoader
    {
        using AssetManager = ::ddknd::asset::AssetManager;
        template <typename Tag>
        using AssetID = ::ddknd::asset::AssetID<Tag>;

        using ShaderTag = ::ddknd::asset::tag::Shader;
        using ModelTag = ::ddknd::asset::tag::Model;

        using ShaderID = AssetID<ShaderTag>;
        using ModelID = AssetID<ModelTag>;

      public:
        using AnimationAssetStore = ::ddknd::animation::AnimationAssetStore;

        bool LoadShader(AssetManager& assets, GraphicsAssetStore& store, ShaderID id);
        bool LoadModel(AssetManager& assets, GraphicsAssetStore& gfxstore, AnimationAssetStore& animstore, ModelID id);
    };
} // namespace ddknd::graphics
