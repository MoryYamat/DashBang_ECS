#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include <functional>

#include <iostream>

namespace Game::Character::Animation::Profile
{
	using SkillId = uint32_t;
	using AnimId = std::string;
	using CharacterId = std::string;
	constexpr std::string_view kDefaultChar = "Default";


	// 透過検索用
	struct TransparentStringHash {
		using is_transparent = void;// 透過検索対応
		size_t operator()(std::string_view key) const noexcept {
			return std::hash<std::string_view>{}(key);
		}
	};

	// 透過比較器
	//struct TransparentStringEq {
	//	using is_transparent = void;// 透過検索対応
	//	bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
	//		return lhs == rhs;
	//	}
	//};

	struct SkillAnimMeta
	{
		std::string clip;
		float speed = 1.0f;
		float fadeIn = 0.0f;
		float startOffset = 0.0f;// 先頭カット
		float hitStart = 0.0f; // 再生開始からの秒
		float hitEnd = 0.0f;
	};

	struct SkillAnimationProfileDatabase
	{
		// using SkillMap = std::unordered_map<SkillId, AnimId>;
		using SkillMap = std::unordered_map<SkillId, SkillAnimMeta>;
		using CharMap = std::unordered_map <
			CharacterId, SkillMap,
			TransparentStringHash,
			std::equal_to<>// 透過比較器
		>;


		CharMap byCharacter;

		AnimId missingClip = "__MISSING__";
		SkillAnimMeta missingMeta{ missingClip, 1.0f, 0.1f, 0.0f, 0.0f, 0.0f }; // ← ここが実体

		bool AddCharacter(CharacterId id, SkillMap skills)
		{
			auto [it, inserted] = byCharacter.try_emplace(std::move(id), std::move(skills));
			if (!inserted)
			{
				std::cerr << "[SkillProfile] Duplicate character: " << it->first << "\n";
			}
			return inserted;
		}

		//const AnimId* Resolve(std::string_view charId, SkillId skill) const
		//{
		//	auto findIn = [&](std::string_view cid) -> const AnimId*
		//		{
		//			if (auto itC = byCharacter.find(cid); itC != byCharacter.end())
		//			{
		//				if (auto itS = itC->second.find(skill); itS != itC->second.end())
		//					return &itS->second;
		//			}
		//			return nullptr;
		//		};

		//	if (auto p = findIn(charId)) return p;
		//	return findIn(kDefaultChar);
		//}

		//const AnimId& ResolveOrFallback(std::string_view charId, SkillId skill) const
		//{
		//	if (auto p = Resolve(charId, skill)) return *p;
		//	return missingClip;
		//}

		const SkillAnimMeta* Resolve(std::string_view charId, SkillId skill) const
		{
			auto findIn = [&](std::string_view cid) -> const SkillAnimMeta*
				{
					if (auto itC = byCharacter.find(cid); itC != byCharacter.end())
					{
						if (auto itS = itC->second.find(skill); itS != itC->second.end())
							return &itS->second;
					}
					return nullptr;
				};

			if (auto p = findIn(charId)) return p;
			return findIn(kDefaultChar);
		}

		const SkillAnimMeta& ResolveOrFallback(std::string_view charId, SkillId skill) const
		{
			if (auto p = Resolve(charId, skill)) return *p;
			return missingMeta;
		}

		void Reserve(std::size_t chars, std::size_t skillsPerCharHint = 0)
		{
			byCharacter.reserve(chars);
			if (skillsPerCharHint)
			{
				for (auto& [_, sm] : byCharacter)
					sm.reserve(skillsPerCharHint);
			}
		}

		bool AddOrUpdate(CharacterId id, SkillMap skills)
		{
			auto [it, inserted] = byCharacter.insert_or_assign(std::move(id), std::move(skills));
			if (!inserted) std::cerr << "[SkillProfile] Updated character: " << it->first << "\n";
			return inserted;
		}
	};
}