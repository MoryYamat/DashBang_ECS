#pragma once

#include "IEffectTemplate.hpp"

#include "Game/Combat/Skill/Component/SkillOwnerComponent.hpp"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"
#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"
#include "Game/Combat/Skill/Component/HitboxHitMemoComponent.hpp"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Game/ECS/Component/LifetimeComponent.hpp"

// collision
#include "Game/Collision/Component/CollisionMaskComponent.h"

// Team ID
#include "Game/ECS/Component/TeamComponent.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

// TODO: spawnHitAreaに scale / spawnOffset などを拡張して Transform2DComponent の初期化を定義ドリブン化する
// TODO: Lifetime の type を定義に持たせて FSM同期や衝突制御と連携できるようにする
// TODO: CollisionMaskComponentの初期化
namespace Game::Combat::Skill::FSM::Effect
{
	using namespace Game::Combat::Skill::Component;
	using namespace Game::ECS::Component;
	using namespace Engine::ECS::Component::Logic2D;
	
	using namespace Game::Collision::Component;

	struct SpawnHitboxEffect : IEffectTemplate
	{
		void execute(
			Engine::ECS::EntityMgr& ecs,
			Engine::ECS::Entity caster,
			const SkillDef& def,
			const SkillFSMContext& ctx
		) const override
		{
			namespace Ops = Engine::ECS::Ops;
			if (!def.spawnHitArea.has_value()) return;

			const auto& spawn = def.spawnHitArea.value();

			Engine::ECS::Entity eHitbox = ecs.createEntity();


			Ops::Add<Game::Combat::Skill::Component::SkillOwnerComponent>
				(ecs, eHitbox, Game::Combat::Skill::Component::SkillOwnerComponent{.caster = caster, .skillId = def.id});

			// TODO: 
			// FIXME: def に定義されたCollisionMaskをコピーし，追加するように変更する
			// CollisionMask
			Ops::Add<Game::Collision::Component::CollisionMaskComponent>(ecs, eHitbox,
				Game::Collision::Component::CollisionMaskComponent
				{
					.category = Category::SkillHitbox,
					.collideCategoryMask = bit(Category::CharacterBody),
					.relationMask = bit(Relation::Enemy) | bit(Relation::Ally),
					// .relationMask = bit(Relation::Enemy),
					.flags = 0
				});


			// 判定済みtarget
			Ops::Add<Game::Combat::Skill::Component::HitboxHitMemoComponent>(ecs, eHitbox,
				Game::Combat::Skill::Component::HitboxHitMemoComponent{});


			// 当たり判定形状
			Ops::Add<Game::Combat::Skill::Component::Attack2DAreaComponent>
				(ecs, eHitbox, Game::Combat::Skill::Component::Attack2DAreaComponent{
					.shape = spawn.shape
					});


			// 軌跡形状
			Ops::Add<Game::Combat::Skill::Component::SkillTrajectoryComponent>
				(ecs, eHitbox, Game::Combat::Skill::Component::SkillTrajectoryComponent{
				.trajectory = spawn.trajectoryParams
					});

			// ライフタイム(寿命管理)
			Ops::Add<Game::ECS::Component::LifetimeComponent>
				(ecs, eHitbox, Game::ECS::Component::LifetimeComponent{
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

				Ops::Add<Engine::ECS::Component::Logic2D::Transform2DComponent>
					(ecs, eHitbox, initialHitboxTransform);
			}

			// std::cout << "[SpawnHitboxEffect.hpp]: Spawned Hitbox Entity" << eHitbox.id 
			// 	<< " for SkillID = " << def.id 
			// 	<< "by Caster = " << caster.id << "\n";




			// TODO:
			// コリジョンマスクの作成システム実装 
			// 最適化
			//
		}
	};
}