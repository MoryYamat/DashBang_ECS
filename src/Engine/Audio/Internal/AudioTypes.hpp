#pragma once
#include "Engine/Audio/Public/AudioFwd.hpp"


#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace Engine::Audio
{


	// internal
	class AudioAssetDB
	{
	public:
		
		// 追加して SoundIDを返す (連番で払い出す)
		SoundID register_sound(SoundDef def);

		// 取得 (無効IDはnullptr)
		const SoundDef* try_get(SoundID id) const noexcept;

		std::size_t size() const noexcept { return sounds_.size(); }

	private:
		std::vector<SoundDef> sounds_;

	};


	// 透明比較用 ==========================
	struct TransparentStringEqual
	{
		using is_transparent = void;

		bool operator()(std::string_view a, std::string_view b) const noexcept
		{
			return a == b;
		}

		bool operator()(const std::string& a, const std::string& b) const noexcept
		{
			return a == b;
		}

		bool operator()(const std::string& a, std::string_view b) const noexcept
		{
			return a == b;
		}

		bool operator()(std::string_view a, const std::string& b) const noexcept
		{
			return a == b;
		}
	};

	struct TransparentStringHash
	{
		using is_transparent = void;

		std::size_t operator()(std::string_view sv) const noexcept
		{
			return std::hash<std::string_view>{}(sv);
		}

		std::size_t operator()(const std::string& s) const noexcept
		{
			return std::hash<std::string_view>{}(s);
		}
	};
	// ==========================

	class SoundNameTable
	{
	public:
		// 登録 (同名があったら false)
		bool register_name(std::string name, SoundID id);

		// 検索
		SoundID find(std::string_view name) const;

	private:
		std::unordered_map<std::string, SoundID, TransparentStringHash, TransparentStringEqual> name_to_id_;
	};

}