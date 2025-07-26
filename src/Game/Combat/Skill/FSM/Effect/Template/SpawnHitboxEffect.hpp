#pragma once

#include "IEffectTemplate.hpp"

#include "Game/Combat/Skill/Component/SkillOwnerComponent.hpp"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/ECS/Component/LifetimeComponent.hpp"


// TODO: spawnHitAreaに scale / spawnOffset などを拡張して Transform2DComponent の初期化を定義ドリブン化する
// TODO: Lifetime の type を定義に持たせて FSM同期や衝突制御と連携できるようにする
// TODO: CollisionMaskComponentの初期化
namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::ECS::Component;
	using namespace Engine::ECS::Component::Logic2D;
	
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

			if (ecs.hasComponent<Logic2DTransformComponent>(caster))
			{
				const auto& casterTransform = ecs.get<Logic2DTransformComponent>(caster);

				// FixMe：scaleは1.0fに固定しているがスキル定義などにより拡大したり縮小したりできるようにしたほうがいい
				Transform2DComponent initialHitboxTransform;
				initialHitboxTransform.positionXZ = casterTransform.positionXZ;
				initialHitboxTransform.rotationY = casterTransform.GetRotationYFromFrontVector();
				initialHitboxTransform.front = casterTransform.front;
				initialHitboxTransform.right = casterTransform.right;
				initialHitboxTransform.scale = 1.0f;

				ecs.addComponent(eHitbox, initialHitboxTransform);
			}

			std::cout << "[SpawnHitboxEffect.hpp]: Spawned Hitbox Entity" << eHitbox.id 
				<< " for SkillID = " << def.id 
				<< "by Caster = " << caster.id << "\n";




			// TODO:
			// コリジョンマスクの作成システム実装 
			// 最適化
			//
		}
	};
}