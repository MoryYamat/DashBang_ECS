#include "Engine/Audio/Internal/AudioTypes.hpp"
#include "Engine/Audio/Public/AudioAPI.hpp"
#include "Engine/Audio/Public/AudioFwd.hpp"

#include <cstddef>
#include <cstdint>
#include <utility>
#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>


namespace Engine::Audio
{
	struct AudioCatalog::Impl
	{
		AudioAssetDB db;
		SoundNameTable names;
	};

	SoundID AudioAssetDB::register_sound(SoundDef def)
	{
		const std::uint32_t index = static_cast<std::uint32_t>(sounds_.size());
		sounds_.push_back(std::move(def));
		return SoundID{ index };
	}

	const SoundDef* AudioAssetDB::try_get(SoundID id)const noexcept
	{
		if (!id.is_valid())
		{
			return nullptr;
		}

		const auto index = static_cast<std::size_t>(id.value());
		if (index >= sounds_.size())
		{
			return nullptr;
		}

		return &sounds_[index];
	}

	bool SoundNameTable::register_name(std::string name, SoundID id)
	{
		if (!id.is_valid()) return false;

		auto [it, inserted] = name_to_id_.emplace(std::move(name), id);
		return inserted;
	}

	SoundID SoundNameTable::find(std::string_view name) const
	{
		auto it = name_to_id_.find(name);
		if (it == name_to_id_.end())
		{
			return SoundID::Invalid();
		}
		return it->second;
	}

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
		if (impl_->names.find(name).is_valid())
		{
			return SoundID::Invalid();
		}

		SoundID id = impl_->db.register_sound(std::move(def));
		const bool ok = impl_->names.register_name(std::move(name), id);
		if (!ok)
		{
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
