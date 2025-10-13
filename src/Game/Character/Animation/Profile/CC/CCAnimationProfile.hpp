#pragma once

#include <string>
#include <string_view>

#include <typeindex>
#include <unordered_map>
#include <functional>

#include <cstdint>
#include <iostream>

namespace Game::Character::Animation::Profile::CC
{
	// TODO:
	// このstd::type_indexはid対応にしたほうがいいと思われる．が後回し
	using CCId = std::type_index;
	using ProfId = std::string;
	constexpr std::string_view kDefaultChar = "default";

	// 透過検索用
	struct TransparentStringHash {
		using is_transparent = void;// 透過検索対応
		size_t operator()(std::string_view key) const noexcept {
			return std::hash<std::string_view>{}(key);
		}
	};

	struct CCAnimationProfile
	{
		std::string clipKey;
		float speed = 1.0f;
		float fadeIn = 0.0f;
		float startOffset = 0.0f;// 先頭カット
	};

	// db
	struct CCAnimationProfileDatabase
	{
		using CCMap = std::unordered_map<CCId, CCAnimationProfile>;
		using ProfMap = std::unordered_map<
			ProfId,
			CCMap,
			TransparentStringHash,
			std::equal_to<>
		>;

		ProfMap byProfId;

		CCAnimationProfile missingProf{};

		bool AddCCProfile(const ProfId pname, const CCMap ccmap)
		{
			auto [it, inserted] = byProfId.try_emplace(std::move(pname), std::move(ccmap));
			if (!inserted)
			{
				std::cerr << "[CCProfile] Duplicate ccProfile: " << it->first << "\n";
			}
			return inserted;

		}

		const CCAnimationProfile* Resolve(const std::string_view ccProf, CCId ccid) const
		{
			auto findIn = [&](std::string_view ccprof) -> const CCAnimationProfile*
				{
					if (auto itC = byProfId.find(ccprof); itC != byProfId.end())
					{
						if (auto itS = itC->second.find(ccid); itS != itC->second.end())
						{
							return &itS->second;
						}
					}

					return nullptr;
				};

			if (auto p = findIn(ccProf)) return p;
			return findIn(kDefaultChar);
		}

		const CCAnimationProfile& ResolveOrFallback(const std::string_view ccProf, CCId ccid) const
		{
			if (auto p = Resolve(ccProf, ccid)) return *p;
			return missingProf;
		}
	};
}