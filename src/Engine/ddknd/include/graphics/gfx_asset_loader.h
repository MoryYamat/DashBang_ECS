#pragma once

#include "gfx_type.h"
#include "asset/asset_fwd.h"
#include "asset/asset_tag.h"
#include "asset/asset_manager.h"



namespace ddknd::graphics
{
    class GraphicsAssetStore
    {
        public:
            using ShaderID = ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader>;
            using ModelID = ::ddknd::asset::AssetID<::ddknd::asset::tag::Model>;
            using ShaderResource = asset::ShaderResource;
            using ModelResource = asset::ModelRenderResource;

            const ShaderResource* TryGet(ShaderID id) const;
            const ModelResource* TryGet(ModelID id) const;
            
            void SetLoaded(ShaderID id, ShaderResource res);
            void SetLoaded(ModelID id, ModelResource res);
        private:
            template<typename T, typename Tag>
            using Storage = ::ddknd::asset::Storage<T, Tag>;
            
            using ShaderTag = ::ddknd::asset::tag::Shader;
            using ModelTag = ::ddknd::asset::tag::Model;

            Storage<ShaderResource, ShaderTag> shaders_;
            Storage<ModelResource, ModelTag> models_;
    };  

    class GraphicsAssetLoader
    {
        using AssetManager = ::ddknd::asset::AssetManager;
        using ShaderID = ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader>;
        using ModelID = ::ddknd::asset::AssetID<::ddknd::asset::tag::Model>;

      public:
        bool LoadShader(AssetManager& assets, GraphicsAssetStore& store, ShaderID id);
        bool LoadModel(AssetManager& assets, GraphicsAssetStore& store, ModelID id);
    };
} // namespace ddknd::graphics