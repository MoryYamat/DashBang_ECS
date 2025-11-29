#include "SkillFeature.h"



#include "Game/Combat/Private/Skill/System/Trigger/PlayerSkillTriggerSystem.h"

#include "Game/Combat/Private/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Private/Skill/System/UpdateSkillPhase.h"// 削除予定：FSM導入後廃止
#include "Game/Combat/Private/Skill/System/SkillTrajectorySystem.h"

#include "Game/Combat/Private/Skill/System/InitializeSkills.h"// 削除予定：FSM導入後廃止

#include "Game/Combat/Private/Skill/FSM/Init/InitSkillDatabase.hpp"// 最新

#include "Game/Combat/Private/Skill/System/Lifetime/SkillExecutionLifetimeSystem.hpp"// 最新

// lifetime
#include "Game/Combat/Private/Skill/System/HitArea/Attack2DAreaLifetimeSystem.hpp"

#include "Game/Combat/Private/Skill/System/Lifetime/HitboxLifetimeControlSystem.hpp"

// lifetimeComponent導入後未使用：削除予定
#include "Game/Combat/Private/Skill/System/Phase/AttackLifetimeSystem.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include "Game/Combat/Skill/Public/SkillApi.hpp"

//
//
namespace Game::Feature::Combat
{
	void SkillFeature::InitSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::InitAllSkillSystem(ctx);
	}

	void SkillFeature::UpdateSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Combat::Skill::UpdateAllSkillRuntimeSystem(ctx);
	}
}