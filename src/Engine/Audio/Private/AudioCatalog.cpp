#include "Engine/Audio/Internal/AudioTypes.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Engine/Audio/Public/AudioFwd.hpp"

#include "Engine/Audio/Internal/AudioLog.hpp"

#include <cstddef>
#include <cstdint>
#include <utility>
#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>


namespace Engine::Audio
{




	// ========================= AUDIO CATALOG ======================
	struct AudioCatalog::Impl
	{
		AudioAssetDB db;
		SoundNameTable names;
	};

	AudioCatalog::AudioCatalog()
		:impl_(std::make_unique<Impl>())
	{

	}

	AudioCatalog::~AudioCatalog()
	{
		impl_.reset();
	}

	AudioCatalog::AudioCatalog(AudioCatalog&& other) noexcept
		: impl_(std::move(other.impl_))
	{
		other.impl_.reset();
	}

	AudioCatalog& AudioCatalog::operator=(AudioCatalog&& other) noexcept
	{
		if (this != &other)
		{
			impl_.reset();
			impl_ = std::move(other.impl_);
			other.impl_.reset();
		}

		return *this;
	}

	SoundID AudioCatalog::register_sound(std::string name, SoundDef def)
	{
		// ä˘Ç…ë∂ç›Ç∑ÇÈñºëOÇ»ÇÁé∏îs(çƒìoò^ã÷é~)
		if (impl_->names.find(std::string_view{ name }).is_valid())
		{
			// error log
			const SoundID existing = impl_->names.find(std::string_view{ name });
			std::string msg = "Duplicate sound name: '" + name + "' existing_id=" + std::to_string(existing.value());
			Log::warn(Log::kCatalog, msg.c_str());


			return SoundID::Invalid();
		}

		SoundID id = impl_->db.register_sound(std::move(def));
		const bool ok = impl_->names.register_name(std::move(name), id);
		if (!ok)
		{
			Log::error(Log::kCatalog, "register_name failed (unexpected)");

			return SoundID::Invalid();
		}

		return id;
	}

	SoundID AudioCatalog::find(std::string_view name) const noexcept
	{
		return impl_->names.find(name);
	}

	const SoundDef* AudioCatalog::try_get(SoundID id) const noexcept
	{
		return impl_->db.try_get(id);
	}
}

// é¿çsånÇ≈ÇÕÅAidÇ≈âåàÇ∑ÇÈ
// 
// struct CombatSounds
// {
// 	SoundID hit_light;
// 	SoundID hit_heavy;
// 	SoundID swing;
// };
// 
// CombatSounds sounds;
// sounds.hit_light = catalog.find("HitLight");
// sounds.hit_heavy = catalog.find("HitHeavy");
// sounds.swing = catalog.find("Swing");
// 
// cmd.PlaySound(sounds.hit_light, pos, volumeScale);
