// 削除予定：FSMシステム実装後廃止

#include "UpdateSkillPhase.h"

#include "Game/Combat/Private/Skill/System/SkillCastingSystem.h"
#include "Game/Combat/Private/Skill/System/UpdateSkillLifetimes.h"

// 削除予定
#include "Game/Combat/Private/Skill/Component/SkillInstanceComponent.h"

#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"

#include "Engine/ECS/Component/Tags/PendingDestroyComponent.h"

#include "Game/Character/Private/State/Component/CCStateComponent.hpp"

#include "Engine/ECS/EntityUtils/EntityUtils.h"


// 削除予定：FSM実装後廃止予定
// Fixme: スキルの種類によってSkillPhaseの更新処理を分ける必要があるかもしれない
void Game::Combat::Skill::System::UpdateSkillPhase(Engine::ECS::EntityMgr& ecs, float deltaTime)
{
	//Game::Combat::Skill::Data::SkillDatabase& skillDB = ecs.getResource<Game::Combat::Skill::Data::SkillDatabase>();

	//for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillExecutionComponent>())
	//{
	//	auto& execution = ecs.get<Game::Combat::Skill::Component::SkillExecutionComponent>(e);
	//	const auto& def = skillDB.Get(execution.skillId);

	//	execution.timeSinceCast += deltaTime;
	//	execution.phaseElapsedTime += deltaTime;

	//	switch (execution.currentPhase)
	//	{
	//	case Game::Combat::Skill::Component::SkillExecutionPhase::Casting:
	//		// 中間計算式の導入によるキャラクターステータスの反映も検討
	//		if (execution.phaseElapsedTime >= def.execution.timing.castTime)
	//		{
	//			execution.currentPhase = Game::Combat::Skill::Component::SkillExecutionPhase::Active;
	//			execution.phaseElapsedTime = 0.0f;// リセット
	//			// Game::Combat::Skill::System::spawnSkillHitArea(ecs, skillDB, e);// 攻撃範囲生成
	//		}
	//		break;

	//	case Game::Combat::Skill::Component::SkillExecutionPhase::Active:
	//		if (execution.phaseElapsedTime >= def.execution.timing.duration)
	//		{
	//			execution.currentPhase = Game::Combat::Skill::Component::SkillExecutionPhase::Recovery;
	//			execution.phaseElapsedTime = 0.0f;
	//		}
	//		break;

	//	case Game::Combat::Skill::Component::SkillExecutionPhase::Recovery:
	//		if (execution.phaseElapsedTime >= def.execution.timing.recoveryTime)
	//		{
	//			execution.currentPhase = Game::Combat::Skill::Component::SkillExecutionPhase::Completed;
	//		}
	//		break;

	//	case Game::Combat::Skill::Component::SkillExecutionPhase::Completed:
	//	case Game::Combat::Skill::Component::SkillExecutionPhase::Canceled:
	//	case Game::Combat::Skill::Component::SkillExecutionPhase::Interrupted:
	//		// ↓↓↓古の関数(削除予定)↓↓↓
	//		// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);

	//		std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << execution.skillId << std::endl;
	//		
	//		// FixMe: このMarkも別のシステムから行えばいいと思う．つまりここはPhaseを更新するだけのシステムにすればいい
	//		//Engine::ECS::EntityUtils::MarkForPendingDestroy(ecs, e); // スキルインスタンスを削除



	//		// Engine::ECS::EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);


	//		// addPendingDestroyComp(ecs, e, instance);

	//		break;
	//	}


	//}
}

// 廃止予定：SkillInstanceComponentの廃止に伴う
// Fixme: スキルの種類によってSkillPhaseの更新処理を分ける必要があるかもしれない
//void Game::Combat::Skill::System::UpdateSkillPhase(Engine::ECS::EntityMgr& ecs, float deltaTime)
//{
//	Game::Combat::Skill::Data::SkillDatabase& skillDB = ecs.getResource<Game::Combat::Skill::Data::SkillDatabase>();
//
//	for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
//	{
//		auto& instance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(e);
//		const auto& def = skillDB.Get(instance.skillId);
//
//		instance.timeSinceCast += deltaTime;
//
//		switch (instance.phase)
//		{
//		case Game::Combat::Skill::Component::SkillPhase::Casting:
//			if (instance.timeSinceCast >= def.execution.timing.castTime && !instance.hasSpawned)
//			{
//				instance.phase = Game::Combat::Skill::Component::SkillPhase::Active;
//				instance.timeSinceCast = 0.0f;// リセット
//				Game::Combat::Skill::System::spawnSkillHitArea(ecs, skillDB, e);// 攻撃範囲生成
//				instance.hasSpawned = true;
//			}
//			break;
//
//		case Game::Combat::Skill::Component::SkillPhase::Active:
//			if (instance.timeSinceCast >= def.execution.timing.duration)
//			{
//				instance.phase = Game::Combat::Skill::Component::SkillPhase::Recovery;
//				instance.timeSinceCast = 0.0f;
//			}
//			break;
//
//		case Game::Combat::Skill::Component::SkillPhase::Recovery:
//			if (instance.timeSinceCast >= def.execution.timing.recoveryTime)
//			{
//				instance.phase = Game::Combat::Skill::Component::SkillPhase::Completed;
//			}
//			break;
//
//		case Game::Combat::Skill::Component::SkillPhase::Completed:
//			// ↓↓↓古の関数(削除予定)↓↓↓
//			// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);
//			
//			if (!instance.isSkillCompleted && !IsSkillInterrupted(instance, ecs))
//			{
//				std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << instance.skillId << std::endl;
//				instance.isSkillCompleted = true; // フラグを立てて重複削除防止
//				Engine::ECS::EntityUtils::MarkForPendingDestroy(ecs, e); // スキルインスタンスを削除
//			}
//
//
//			// Engine::ECS::EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);
//
//
//			// addPendingDestroyComp(ecs, e, instance);
//
//			break;
//		}
//
//
//	}
//}

// 
bool Game::Combat::Skill::System::AllAttacksDestroyed(Engine::ECS::EntityMgr& ecs, const Game::Combat::Skill::Component::SkillInstanceComponent& instance)
{
	for (const Engine::ECS::Entity& hitArea : instance.spawnedHitAreas)
	{
		if (ecs.isAlive(hitArea) && ecs.hasComponent<Game::Combat::Skill::Component::Attack2DAreaComponent>(hitArea))
		{
			return false; // まだ攻撃エリアが存在する
		}
	}
	return true; // すべての攻撃エリアが削除された
}

// スキル中断の判定 (今後拡張する)
bool Game::Combat::Skill::System::IsSkillInterrupted(const Game::Combat::Skill::Component::SkillInstanceComponent& instance, Engine::ECS::EntityMgr& ecs)
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
void Game::Combat::Skill::System::UpdateSkillPhase(Engine::ECS::EntityMgr& ecs, float deltaTime, Game::Combat::Skill::Data::SkillDatabase& skillDB)
{
	for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	{
		auto& instance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(e);
		const auto& def = skillDB.Get(instance.skillId);

		instance.timeSinceCast += deltaTime;

		switch (instance.phase)
		{
		case Game::Combat::Skill::Component::SkillPhase::Casting:
			if (instance.timeSinceCast >= def.execution.timing.castTime)
			{
				instance.phase = Game::Combat::Skill::Component::SkillPhase::Active;
				instance.timeSinceCast = 0.0f;// リセット
				Game::Combat::Skill::System::SpawnSkillHitArea(ecs, skillDB);// 攻撃範囲生成
			}
			break;

		case Game::Combat::Skill::Component::SkillPhase::Active:
			if (instance.timeSinceCast >= def.execution.timing.duration)
			{
				instance.phase = Game::Combat::Skill::Component::SkillPhase::Recovery;
				instance.timeSinceCast = 0.0f;
			}
			break;

		case Game::Combat::Skill::Component::SkillPhase::Recovery:
			if (instance.timeSinceCast >= def.execution.timing.recoveryTime)
			{
				instance.phase = Game::Combat::Skill::Component::SkillPhase::Completed;
			}
			break;

		case Game::Combat::Skill::Component::SkillPhase::Completed:
			// ↓↓↓古の関数(削除予定)↓↓↓
			// SkillSystem::Lifetime::CleanUpCompletedSkills(ecs);
			std::cout << "[UpdateSkillPhase.cpp(Completed Skill)] entity id " << instance.skillId << std::endl;
			
			Engine::ECS::EntityUtils::MarkForPendingDestroyWithChildren(ecs, e, instance.spawnedHitAreas);
			
			// addPendingDestroyComp(ecs, e, instance);

			break;
		}


	}
}

