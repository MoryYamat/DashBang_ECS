#pragma once

#include <string>
#include <unordered_map>

namespace Game::Character::Animation::Profile
{
	struct MovementAnimationProfile
	{
		std::string idle;
		std::string runFwd;
		std::string runBack;
		std::string runRight;
		std::string runLeft;
	};


	struct MovementAnimationProfileDatabase
	{
		std::unordered_map<std::string, MovementAnimationProfile> movements;

		const MovementAnimationProfile* FindMovement(const std::string& id) const
		{
			auto it = movements.find(id);
			return (it == movements.end()) ? nullptr : &it->second;
		}
	};
}