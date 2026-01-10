#include "Engine/IO/Public/FileSystemAPI.hpp"
#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Platform/Pubilc/PlatformAPI.hpp"


namespace Engine::IO
{
	void InitFileSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& table = ctx.ww.CreateResource<MountTable>();
		
		auto exeDir = Engine::Platform::GetExecutableDirectory();
		auto assetDir = FindAncestoryDirectory(exeDir, "Assets");

		if (assetDir.empty())
		{
			// Log
		}

		table.Mount("res", assetDir);


		std::cerr << "path: " << assetDir << "\n";
	}
}