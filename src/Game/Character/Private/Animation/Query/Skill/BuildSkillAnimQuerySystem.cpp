#include "BuildSkillAnimQuerySystem.hpp"


#include "Game/Character/Private/Animation/Query/Skill/SkillAnimationQueryComponent.hpp"

#include "Game/Combat/Private/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Private/Skill/Def/SkillDef.hpp"


#include "Game/Combat/Private/Skill/FSM/SkillStateTags.hpp"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

namespace Game::Character::Animation::Query
{
	namespace Ops = Engine::ECS::Ops;
	namespace Logic2D = Engine::ECS::Component::Logic2D;
	namespace SkillDef = Game::Combat::Skill::Def;// 不要? // skillId -> animation (id で対応) 
	namespace Skill = Game::Combat::Skill::FSM::StateModel;
	namespace StateTag = Game::Combat::Skill::FSM::StateTag;
	namespace SkillComp = Game::Combat::Skill::Component;

	void BuildSkillAnimQuerySystem(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			Skill::SkillStateComponent,
			SkillComp::SkillExecutionContextComponent,
			SkillAnimQueryComponent,
			Logic2D::Logic2DTransformComponent
		>())
		{
			const auto& state = Ops::Get<Skill::SkillStateComponent>(ecs, e);
			const auto& sCtx = Ops::Get<SkillComp::SkillExecutionContextComponent>(ecs, e);
			const auto& logic = Ops::Get<Logic2D::Logic2DTransformComponent>(ecs, e);
			auto& q = Ops::Get<SkillAnimQueryComponent>(ecs, e);

			// query
			q.currentPhase = state.current;
			q.skillId = sCtx.skillId;
			q.elapsedTime = sCtx.elapsedTime;
			q.phaseElapsedTime = sCtx.phaseElapsedTime;
			q.isInterrupted = sCtx.isInterrupted;
			q.isSkillActive = (state.current != StateTag::NONE 
				&& state.current != StateTag::COMPLETED && state.current != StateTag::INTERRUPTED);
			q.valid = true;
		}
	}
}