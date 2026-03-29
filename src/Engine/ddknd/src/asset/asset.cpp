#include "asset/asset.h"

#include "internal/asset/resource_loader.h"

#include "io/io.h"
#include "graphics/renderer.h"

#include <iostream>

namespace ddknd::asset
{
    AssetManager::AssetManager(const ::ddknd::io::IPathResolver& vfs, ddknd::graphics::IRendererBackend& gfxBackend)
        : vfs_(vfs), gfxBackend_(gfxBackend)
        {}


    bool AssetManager::LoadNowShader(ID<asset::type::ShaderResource> id)
	{
		auto& storage = shaders_;

		if (storage.GetState(id) == AssetState::Loaded)
			return true;

		if (!storage.TryMarkLoading(id))
			return storage.GetState(id) == AssetState::Loaded;

		const auto* h = storage.TryGetHeader(id);
		if (!h)
		{
			std::cerr << "failed_01\n";
			storage.SetFailed(id);
			return false;
		}

		auto res = ddknd::asset::loader::ResourceLoader<asset::type::ShaderResource>::Load(vfs_, gfxBackend_, h->vpath);


		if (!res)
		{
			std::cerr << "failed_02\n";
			storage.SetFailed(id);
			return false;
		}

		storage.SetLoaded(id, std::move(res));
		return true;

	}

	bool AssetManager::LoadNowMesh(ID<asset::type::MeshResource> id)
	{
		auto& storage = meshes_;

		if (storage.GetState(id) == AssetState::Loaded)
		{
			return true;
		}

		if (!storage.TryMarkLoading(id))
			return storage.GetState(id) == AssetState::Loaded;

		const auto* h = storage.TryGetHeader(id);
		if (!h)
		{
			storage.SetFailed(id);
			return false;
		}

		auto res = ddknd::asset::loader::ResourceLoader<ddknd::asset::type::MeshResource>::Load(vfs_, gfxBackend_, h->vpath);

		if(!res)
		{
			storage.SetFailed(id);
			return false;
		}

		storage.SetLoaded(id, std::move(res));
		return true;
	}

} // namespace ddknd::asset