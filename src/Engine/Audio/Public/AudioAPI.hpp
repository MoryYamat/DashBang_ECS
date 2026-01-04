#pragma once
#include "Engine/Audio/Public/AudioFwd.hpp"

#include <string>
#include <string_view>
#include <memory>

namespace Engine::Audio
{
	struct SoundDef
	{
		std::string path;
		AudioBus bus = AudioBus::SFX;
		float defaultVolume = 1.0f;
		bool stream = false;			// true -> streaming 
	};

	// public
	class AudioCatalog
	{
	public:
		AudioCatalog();
		~AudioCatalog();
		 
		AudioCatalog(AudioCatalog&&) noexcept;
		AudioCatalog& operator=(AudioCatalog&&)noexcept;

		// ÉRÉsÅ[ã÷é~
		AudioCatalog(const AudioCatalog&) = delete;
		AudioCatalog& operator=(const AudioCatalog&) = delete;


		SoundID register_sound(std::string name, const SoundDef def);
		const SoundDef* try_get(SoundID id) const noexcept;
		SoundID find(std::string_view name) const noexcept;

	private:
		struct Impl;
		std::unique_ptr<Impl> impl_;
	};
}