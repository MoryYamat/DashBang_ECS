#include "Engine/Audio/Public/AudioAPI.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


namespace Engine::Audio
{
	void RegisterAudioResources(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& audioCtx = ctx.ww.CreateResource<AudioContext>();
		auto& catalog = audioCtx.res.catalog;

		// CMDBuffer‚ÍŒã‰ñ‚µ

		// 
		audioCtx.res.ids.sfx_test = catalog.register_sound(
			"sfx.test",
			SoundDef{ "Assets/Sounds/test.wav", AudioBus::SFX, 1.0f, false }
		);
	}

	// –¢Žg—p
	void InitAllAudioSystem(::Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		// ‰Šú‰»‡ register -> audiosystem

		RegisterAudioResources(ctx);
	}
}