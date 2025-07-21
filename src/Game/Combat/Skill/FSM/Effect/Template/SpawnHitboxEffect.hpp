#pragma once

#include "IEffectTemplate.hpp"

#include "Game/Combat/Skill/Component/SkillOwnerComponent.hpp"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"

#include "Game/ECS/Component/LifetimeComponent.hpp"

namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::ECS::Component;

	struct SpawnHitboxEffect : IEffectTemplate
	{
		void execute(
			eNsECS::EntityMgr& ecs,
			eNsECS::Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx
		) const override
		{
			if (!def.spawnHitArea.has_value()) return;

			const auto& spawn = def.spawnHitArea.value();

			eNsECS::Entity eHitbox = ecs.createEntity();

			ecs.addComponent(eHitbox, SkillOwnerComponent{
				.caster = caster,
				.skillId = def.id
				});


			// 当たり判定形状
			ecs.addComponent(eHitbox, Attack2DAreaComponent{
				.shape = spawn.shape
				});


			// 軌跡形状
			ecs.addComponent(eHitbox, SkillTrajectoryComponent{
				.trajectory = spawn.trajectoryParams
				});

			// ライフタイム(寿命管理)
			ecs.addComponent(eHitbox, LifetimeComponent{
				.totalLifetime = spawn.duration.value_or(0.0f),
				.elapsedTime = 0.0f,
				});

			std::cout << "[SpawnHitboxEffect.hpp]: Spawned Hitbox Entity" << eHitbox.id << " for SkillID = " << def.id << "by Caster = " << caster.id << "\n";

			// TODO:
			// 定義ドリブンの厳密化
			// lifetime管理の高度化
			// システム実装
			// 最適化
			//
		}
	};
}