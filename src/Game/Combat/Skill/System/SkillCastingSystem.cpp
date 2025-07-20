// 
#include "SkillCastingSystem.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"
#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"



#include "Game/Combat/Skill/MasterData/SkillDefinition.h"

#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"
#include "Game/Combat/Skill/Factory/SkillTrajectoryFactory.h"

// Game ECS
#include "Game/ECS/Component/TeamComponent.h"
#include "Game/ECS/Component/LifetimeComponent.hpp"

#include "Engine/Math/MathUtils.h"

#include "Engine/Debug/DebugUtils.h"


// 削除予定：型ベースFSM導入後廃止予定
void Game::Combat::Skill::System::spawnSkillHitArea(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB, eNsECS::Entity skillEntity)
{
	//auto& instance = ecs.get<gNsSkillComp::SkillInstanceComponent>(skillEntity);

	//std::cout << "[SkillCastingSystem] Checking if SkillID " << instance.skillId << " exists..." << std::endl;
	//if (!skillDB.Has(instance.skillId)) {
	//	std::cout << "[SkillCastingSystem] SkillID not found, skipping." << std::endl;
	//}

	//const gNsSkillData::SkillDefinition& def = skillDB.Get(instance.skillId);

	//auto& transform = ecs.get<eNsLogic2DComp::Transform2DComponent>(skillEntity);
	//const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(instance.caster);
	//const auto& casterTeam = ecs.get<gNsECSComp::TeamComponent>(instance.caster).team;

	//// world 変換
	//glm::vec2 worldCenter = logic.positionXZ;
	//gNsSkillComp::Attack2DShape shape = def.attackSpec.shape;


	//// 攻撃範囲エンティティ生成
	//eNsECS::Entity attack = ecs.createEntity();
	//gNsSkillComp::Attack2DAreaComponent area;
	//area.shape = shape;							// 攻撃範囲の形状を設定

	//// Fixme: lifetimeは`LifetimeComponent`に責務分離したため削除予定
	//area.lifetime = def.attackSpec.lifetime.duration.value_or(0.0f);	// 攻撃範囲のライフタイムを設定

	//area.owner = instance.caster;				// 攻撃範囲の所有者を設定
	//area.skillEntity = skillEntity;				// スキルエンティティを設定
	//std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] create: " << def.name << " is created. \n";
	//ecs.addComponent(attack, area);

	//// Fixme : 関数として切り出したい
	//// Fixme : 関数として切り出したい
	//// Fixme : 関数として切り出したい
	//gNsECSComp::LifetimeComponent lifetime;
	//if (def.attackSpec.lifetime.hitBoxlifetimePolicy == gNsSkillData::AttackLifeTimeMode::SyncWithSkillPhase)
	//{
	//	lifetime.totalLifetime = def.attackSpec.lifetime.duration.value_or(0.0f);	// スキルフェーズの持続時間と同期
	//}
	//else if (def.attackSpec.lifetime.hitBoxlifetimePolicy == gNsSkillData::AttackLifeTimeMode::IndependentEntityLifetime)
	//{
	//	// 攻撃範囲のライフタイムは独立しているため、ここでは特に設定しない
	//	lifetime.totalLifetime = def.attackSpec.lifetime.duration.value_or(0.0f); // 独立したライフタイムを使用
	//}
	//else if (def.attackSpec.lifetime.hitBoxlifetimePolicy == gNsSkillData::AttackLifeTimeMode::AttachedToChildEntity)
	//{
	//	// 攻撃範囲は子エンティティにアタッチされるため、ここでは特に設定しない
	//}
	//ecs.addComponent(attack, lifetime); // 攻撃範囲にライフタイムコンポーネントを追加
	//// Fixme : 関数として切り出したい
	//// Fixme : 関数として切り出したい
	//// Fixme : 関数として切り出したい

	//instance.spawnedHitAreas.push_back(attack);// 生成された攻撃範囲をSkillInstanceに追加

	//// transformを複製
	//ecs.addComponent(attack, transform);

	//eNsDebugLog::LogVector_string("skillCastingSystem.cpp()", transform.positionXZ);

	//// 攻撃判定 Entityに軌跡コンポーネントを追加
	//gNsSkillComp::SkillTrajectoryComponent traj;
	//traj.type = def.attackSpec.trajectoryType;
	//traj.elapsedTime = 0.0f;
	//traj.trajectoryFunc = gNsSkillFactory::SkillTrajectoryFactory::Create(def, transform);
	//ecs.addComponent(attack, traj);

	//// コリジョンマスクを作成
	//gNsCollComp::CollisionMaskComponent mask;
	//mask = generateSkillCollisionMask(ecs, instance.caster);
	//ecs.addComponent(attack, mask);

}


// 削除予定：型ベースFSM導入後廃止予定
//gNsCollComp::CollisionMaskComponent Game::Combat::Skill::System::generateSkillCollisionMask(
//	eNsECS::EntityMgr& ecs,
//	eNsECS::Entity caster
//)
//{
//	gNsCollComp::CollisionMaskComponent mask;
//	//// 将来的には，このselfLayerもskillDefinitionから自動設定する必要があると思われる
//	//// 将来的には，このselfLayerもskillDefinitionから自動設定する必要があると思われる
//	//// 将来的には，このselfLayerもskillDefinitionから自動設定する必要があると思われる
//	//mask.selfLayer = gNsCollData::Layer::Skill;
//
//	//gNsECSComp::Team team = ecs.get<gNsECSComp::TeamComponent>(caster).team;
//
//	//switch (team)
//	//{
//	//case gNsECSComp::Team::PlayerTeam:
//	//	mask.collidesWithMask = static_cast<uint32_t>(gNsCollData::Layer::Enemy | gNsCollData::Layer::Neutral);
//	//	break;
//	//case gNsECSComp::Team::EnemyTeam:
//	//	mask.collidesWithMask = static_cast<uint32_t>(gNsCollData::Layer::Player | gNsCollData::Layer::Neutral | gNsCollData::Layer::Friendly);
//	//	break;
//	//case gNsECSComp::Team::Neutral:
//	//	mask.collidesWithMask = static_cast<uint32_t>(gNsCollData::Layer::Enemy | gNsCollData::Layer::Player | gNsCollData::Layer::Friendly);
//	//	break;
//	//}
//
//	return mask;
//}


// 削除予定：型ベースFSM導入後廃止予定
// (廃止 (呼び出し型をグローバルではなく，ローカルに変更))攻撃判定計上を生成(ECSグローバルリソース(スキル定義のデータベース)を使用した実装) 
void Game::Combat::Skill::System::SpawnSkillHitArea(eNsECS::EntityMgr& ecs)
{
	//gNsSkillData::SkillDatabase& skillDB = ecs.getResource<gNsSkillData::SkillDatabase>();

	//for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	//{
	//	auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);


	//	// (状態レイヤー導入時変更予定 時間による制御はおかしいため)初回発動時のみ
	//	if (skillInstance.timeSinceCast > 0.0f)
	//		continue;


	//	std::cout << "[SkillCastingSystem] Checking if SkillID " << skillInstance.skillId << " exists..." << std::endl;
	//	if (!skillDB.Has(skillInstance.skillId)) {
	//		std::cout << "[SkillCastingSystem] SkillID not found, skipping." << std::endl;
	//		continue;
	//	}



	//	if (!skillDB.Has(skillInstance.skillId)) continue;
	//	const gNsSkillData::SkillDefinition& def = skillDB.Get(skillInstance.skillId);

	//	auto& transform = ecs.get<eNsLogic2DComp::Transform2DComponent>(e);
	//	const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(skillInstance.caster);

	//	// world 変換
	//	glm::vec2 worldCenter = logic.positionXZ;
	//	gNsSkillComp::Attack2DShape shape = def.attackSpec.shape;


	//	// 攻撃範囲エンティティ生成
	//	eNsECS::Entity attack = ecs.createEntity();
	//	gNsSkillComp::Attack2DAreaComponent area;
	//	area.shape = shape;
	//	area.lifetime = def.attackSpec.lifetime.duration.value_or(0.0f);
	//	area.owner = skillInstance.caster;
	//	area.skillEntity = e;
	//	std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] create: " << def.name << " is created. \n";
	//	ecs.addComponent(attack, area);

	//	skillInstance.spawnedHitAreas.push_back(attack);

	//	// transformを複製
	//	ecs.addComponent(attack, transform);

	//	// 攻撃判定 Entityに軌跡コンポーネントを追加
	//	gNsSkillComp::SkillTrajectoryComponent traj;
	//	traj.type = def.attackSpec.trajectoryType;
	//	traj.elapsedTime = 0.0f;
	//	traj.trajectoryFunc = gNsSkillFactory::SkillTrajectoryFactory::Create(def, transform);
	//	ecs.addComponent(attack, traj);
	//}
	//// std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] Hit Area is created successfully\n";

}

// 攻撃判定形状を作成(昔仕様：ECSグローバルリソース実装後廃止予定)
void Game::Combat::Skill::System::SpawnSkillHitArea(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB)
{
	//for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	//{
	//	auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);


	//	// 初回発動時のみ
	//	if (skillInstance.timeSinceCast > 0.0f)
	//		continue;

	//	std::cout << "[SkillCastingSystem] skillId: " << skillInstance.skillId << std::endl;

	//	if (!skillDB.Has(skillInstance.skillId)) continue;
	//	const gNsSkillData::SkillDefinition& def = skillDB.Get(skillInstance.skillId);
	//	auto& transform = ecs.get<eNsLogic2DComp::Transform2DComponent>(e);
	//	const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(skillInstance.caster);

	//	// world 変換
	//	glm::vec2 worldCenter = logic.positionXZ;
	//	gNsSkillComp::Attack2DShape shape = def.attackSpec.shape;


	//	// ローカル形状情報とワールド変換情報を分離したためこの部分の分岐は不要
	//	// Since the local shape information and the world transformation information have been separated, 
	//	// this branch is no longer necessary.
	//	//if (def.shape.IsCircle())
	//	//{
	//	//	// Local shape information
	//	//	Circle2DAttack circle = def.shape.AsCircle();
	//	//	shape = Attack2DShape{ circle };

	//	//	// Update the World Coordinate Information
	//	//	// glm::vec2 offset = MathUtils::RotateVec2(circle.localcenter, transform.rotationY) * transform.scale;
	//	//	// transform.positionXZ = transform.positionXZ + offset;
	//	//}
	//	//else if (def.shape.IsSector())
	//	//{
	//	//	// Local shape information
	//	//	Sector2DAttack sector = def.shape.AsSector();
	//	//	//glm::vec2 direct = transform.GetFrontXZ();
	//	//	//sector.localdirection = direct;
	//	//	shape = Attack2DShape{ sector };

	//	//	// Update the World Coordinate Information
	//	//	// glm::vec2 offset = MathUtils::RotateVec2(sector.localcenter, transform.rotationY) * transform.scale;
	//	//	//transform.positionXZ = transform.positionXZ + offset;

	//	//	// DebugUtils::LogVector_string("SkillCastingSystem.cpp(direction)", sector.localdirection);
	//	//}
	//	//else if (def.shape.IsRectangle())
	//	//{
	//	//	// Local shape information
	//	//	Rectangle2DAttack rect = def.shape.AsRectangle();
	//	//	// rect.localdirection = glm::normalize(MathUtils::RotateVec2(rect.localdirection, transform.rotationY));
	//	//	shape = Attack2DShape{ rect };

	//	//	// Update the World Coordinate Information
	//	//	// glm::vec2 offset = MathUtils::RotateVec2(rect.localcenter, transform.rotationY) * transform.scale;
	//	//	//transform.positionXZ = transform.positionXZ + offset;
	//	//}

	//	// 攻撃範囲エンティティ生成
	//	eNsECS::Entity attack = ecs.createEntity();
	//	gNsSkillComp::Attack2DAreaComponent area;
	//	area.shape = shape;
	//	area.lifetime = def.attackSpec.lifetime.duration.value_or(0.0f);
	//	area.owner = skillInstance.caster;
	//	area.skillEntity = e;
	//	std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] create: " << def.name << " is created. \n";
	//	ecs.addComponent(attack, area);

	//	skillInstance.spawnedHitAreas.push_back(attack);

	//	// transformを複製
	//	ecs.addComponent(attack, transform);

	//	// 攻撃判定 Entityに軌跡コンポーネントを追加
	//	gNsSkillComp::SkillTrajectoryComponent traj;
	//	traj.type = def.attackSpec.trajectoryType;
	//	traj.elapsedTime = 0.0f;
	//	traj.trajectoryFunc = gNsSkillFactory::SkillTrajectoryFactory::Create(def, transform);
	//	ecs.addComponent(attack, traj);
	//}
}
