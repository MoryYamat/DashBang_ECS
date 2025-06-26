#include "UpdateSkillPhase.h"

#include "Game/Combat/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Skill/System/UpdateSkillLifetimes.h"

#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Engine/ECS/Component/Tags/PendingDestroyComponent.h"

#include "Game/Character/State/Component/CCStateComponent.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

// Fixme: スキルの種類によってSkillPhaseの更新処理を分ける必要があるかもしれない
void Game::Combat::Skill::System::UpdateSkillPhase(eNsECS::EntityMgr& ecs, float deltaTime)
{
	gNsSkillData::SkillDatabase& skillDB = ecs.getResource<gNsSkillData::SkillDatabase>();

	for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		auto& instance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);
		const auto& def = skillDB.Get(instance.skillId);

		instance.timeSinceCast += deltaTime;

		switch (instance.phase)
		{
		case gNsSkillComp::SkillPhase::Casting:
			if (instance.timeSinceCast >= def.phaseTiming.castTime && !instance.hasSpawned)
			{
				instance.phase = gNsSkillComp::SkillPhase::Active;
				instance.timeSinceCast = 0.0f;// リセット
				gNsSkillSystem::spawnSkillHitArea(ecs, skillDB, e);// 攻撃範囲生成
				instance.hasSpawned = true;
			}
			break;

		case gNsSkillComp::SkillPhase::Active:
			if (instance.timeSinceCast >= def.phaseTiming.duration)
			{
				instance.phase = gNsSkillComp::SkillPhase::Recovery;
				instance.timeSinceCast = 0.0f;
			}
			break;

		case gNsSkillComp::SkillPhase::Recovery:
			if (instance.timeSinceCast >= def.phaseTiming.recoveryTime)
			{
				instance.phase = gNsSkillComp::SkillPhase::Completed;
			}
			break;

		case gNsSkillComp::SkillPhase::Completed:
			// ↓↓↓古の関数(削除予定)↓↓↓
			// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);
			
			if (!instance.isSkillCompleted && AllAttacksDestroyed(ecs, instance) && !IsSkillInterrupted(instance, ecs))
			{
				std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << instance.skillId << std::endl;
				instance.isSkillCompleted = true; // フラグを立てて重複削除防止
				eNsECS::EntityUtils::MarkForPendingDestroy(ecs, e); // スキルインスタンスを削除
			}


			// eNsECS::EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);


			// addPendingDestroyComp(ecs, e, instance);

			break;
		}


	}
}

// 
bool Game::Combat::Skill::System::AllAttacksDestroyed(eNsECS::EntityMgr& ecs, const gNsSkillComp::SkillInstanceComponent& instance)
{
	for (const eNsECS::Entity& hitArea : instance.spawnedHitAreas)
	{
		if (ecs.isAlive(hitArea) && ecs.hasComponent<gNsSkillComp::Attack2DAreaComponent>(hitArea))
		{
			return false; // まだ攻撃エリアが存在する
		}
	}
	return true; // すべての攻撃エリアが削除された
}

// スキル中断の判定 (今後拡張する)
bool Game::Combat::Skill::System::IsSkillInterrupted(const gNsSkillComp::SkillInstanceComponent& instance, eNsECS::EntityMgr& ecs)
{
	// キャスターのエンティティが生存しているか確認
	if (!ecs.isAlive(instance.caster)) return true;
	if (instance.isInterrupted) return true;

	//// キャスターがスタンや中断状態か確認
	//if (ecs.hasComponent<gNsStateComp::CharacterStateComponent>(instance.caster))
	//{
	//	const auto& state = ecs.get<gNsStateComp::CharacterStateComponent>(instance.caster);
	//	return state.isStunned || state.isInterrupted; // 状態によっては中断扱いにする
	//}

	return false;
}

// スキルの寿命管理システム(ECSのグローバルリソース導入後廃止(現在未使用))
void Game::Combat::Skill::System::UpdateSkillPhase(eNsECS::EntityMgr& ecs, float deltaTime, gNsSkillData::SkillDatabase& skillDB)
{
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		auto& instance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);
		const auto& def = skillDB.Get(instance.skillId);

		instance.timeSinceCast += deltaTime;

		switch (instance.phase)
		{
		case gNsSkillComp::SkillPhase::Casting:
			if (instance.timeSinceCast >= def.phaseTiming.castTime)
			{
				instance.phase = gNsSkillComp::SkillPhase::Active;
				instance.timeSinceCast = 0.0f;// リセット
				gNsSkillSystem::SpawnSkillHitArea(ecs, skillDB);// 攻撃範囲生成
			}
			break;

		case gNsSkillComp::SkillPhase::Active:
			if (instance.timeSinceCast >= def.phaseTiming.duration)
			{
				instance.phase = gNsSkillComp::SkillPhase::Recovery;
				instance.timeSinceCast = 0.0f;
			}
			break;

		case gNsSkillComp::SkillPhase::Recovery:
			if (instance.timeSinceCast >= def.phaseTiming.recoveryTime)
			{
				instance.phase = gNsSkillComp::SkillPhase::Completed;
			}
			break;

		case gNsSkillComp::SkillPhase::Completed:
			// ↓↓↓古の関数(削除予定)↓↓↓
			// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);
			std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << instance.skillId << std::endl;
			
			eNsECS::EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);
			
			// addPendingDestroyComp(ecs, e, instance);

			break;
		}


	}
}

