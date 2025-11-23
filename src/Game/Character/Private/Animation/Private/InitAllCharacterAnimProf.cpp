#include "Game/Character/Private/Animation/Public/AnimationApi.hpp"

#include "Game/Character/Private/Animation/Private/Movement/AnimationProfile.hpp"


#include "Engine/Log/Public/LogApi.hpp"
namespace Game::Character::Animation
{
	void InitAllCharacterAnimProf(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		if (!Movement::InitMoveAnimProfile(ctx))
		{
			Engine::Log::Write(Engine::Log::Level::Error, "[InitMoveAnimProfile]", "Failed to initialize InitMoveAnimProfile");
		}
	}
}