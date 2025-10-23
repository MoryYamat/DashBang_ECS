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

#include "Engine/Math/Private/MathUtils.h"

#include "Engine/Debug/Private/DebugUtils.h"


// 削除予定：型ベースFSM導入後廃止予定
void Game::Combat::Skill::System::spawnSkillHitArea(Engine::ECS::EntityMgr& ecs, Game::Combat::Skill::Data::SkillDatabase& skillDB, Engine::ECS::Entity skillEntity)
{
	//auto& instance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(skillEntity);

	//std::cout << "[SkillCastingSystem] Checking if SkillID " << instance.skillId << " exists..." << std::endl;
	//if (!skillDB.Has(instance.skillId)) {
	//	std::cout << "[SkillCastingSystem] SkillID not found, skipping." << std::endl;
	//}

	//const Game::Combat::Skill::Data::SkillDefinition& def = skillDB.Get(instance.skillId);

	//auto& transform = ecs.get<Engine::ECS::Component::Logic2D::Transform2DComponent>(skillEntity);
	//const auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(instance.caster);
	//const auto& casterTeam = ecs.get<Game::ECS::Component::TeamComponent>(instance.caster).team;

	//// world 変換
	//glm::vec2 worldCenter = logic.positionXZ;
	//Game::Combat::Skill::Component::Attack2DShape shape = def.attackSpec.shape;


	//// 攻撃範囲エンティティ生成
	//Engine::ECS::Entity attack = ecs.createEntity();
	//Game::Combat::Skill::Component::Attack2DAreaComponent area;
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
	//Game::ECS::Component::LifetimeComponent lifetime;
	//if (def.attackSpec.lifetime.hitBoxlifetimePolicy == Game::Combat::Skill::Data::AttackLifeTimeMode::SyncWithSkillPhase)
	//{
	//	lifetime.totalLifetime = def.attackSpec.lifetime.duration.value_or(0.0f);	// スキルフェーズの持続時間と同期
	//}
	//else if (def.attackSpec.lifetime.hitBoxlifetimePolicy == Game::Combat::Skill::Data::AttackLifeTimeMode::IndependentEntityLifetime)
	//{
	//	// 攻撃範囲のライフタイムは独立しているため、ここでは特に設定しない
	//	lifetime.totalLifetime = def.attackSpec.lifetime.duration.value_or(0.0f); // 独立したライフタイムを使用
	//}
	//else if (def.attackSpec.lifetime.hitBoxlifetimePolicy == Game::Combat::Skill::Data::AttackLifeTimeMode::AttachedToChildEntity)
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

	//Engine::Debug::Logging::LogVector_string("skillCastingSystem.cpp()", transform.positionXZ);

	//// 攻撃判定 Entityに軌跡コンポーネントを追加
	//Game::Combat::Skill::Component::SkillTrajectoryComponent traj;
	//traj.type = def.attackSpec.trajectoryType;
	//traj.elapsedTime = 0.0f;
	//traj.trajectoryFunc = Game::Combat::SkillFactory::SkillTrajectoryFactory::Create(def, transform);
	//ecs.addComponent(attack, traj);

	//// コリジョンマスクを作成
	//Game::Collision::Component::CollisionMaskComponent mask;
	//mask = generateSkillCollisionMask(ecs, instance.caster);
	//ecs.addComponent(attack, mask);

}


// 削除予定：型ベースFSM導入後廃止予定
//Game::Collision::Component::CollisionMaskComponent Game::Combat::Skill::System::generateSkillCollisionMask(
//	Engine::ECS::EntityMgr& ecs,
//	Engine::ECS::Entity caster
//)
//{
//	Game::Collision::Component::CollisionMaskComponent mask;
//	//// 将来的には，このselfLayerもskillDefinitionから自動設定する必要があると思われる
//	//// 将来的には，このselfLayerもskillDefinitionから自動設定する必要があると思われる
//	//// 将来的には，このselfLayerもskillDefinitionから自動設定する必要があると思われる
//	//mask.selfLayer = Game::Collision::Data::Layer::Skill;
//
//	//Game::ECS::Component::Team team = ecs.get<Game::ECS::Component::TeamComponent>(caster).team;
//
//	//switch (team)
//	//{
//	//case Game::ECS::Component::Team::PlayerTeam:
//	//	mask.collidesWithMask = static_cast<uint32_t>(Game::Collision::Data::Layer::Enemy | Game::Collision::Data::Layer::Neutral);
//	//	break;
//	//case Game::ECS::Component::Team::EnemyTeam:
//	//	mask.collidesWithMask = static_cast<uint32_t>(Game::Collision::Data::Layer::Player | Game::Collision::Data::Layer::Neutral | Game::Collision::Data::Layer::Friendly);
//	//	break;
//	//case Game::ECS::Component::Team::Neutral:
//	//	mask.collidesWithMask = static_cast<uint32_t>(Game::Collision::Data::Layer::Enemy | Game::Collision::Data::Layer::Player | Game::Collision::Data::Layer::Friendly);
//	//	break;
//	//}
//
//	return mask;
//}


// 削除予定：型ベースFSM導入後廃止予定
// (廃止 (呼び出し型をグローバルではなく，ローカルに変更))攻撃判定計上を生成(ECSグローバルリソース(スキル定義のデータベース)を使用した実装) 
void Game::Combat::Skill::System::SpawnSkillHitArea(Engine::ECS::EntityMgr& ecs)
{
	//Game::Combat::Skill::Data::SkillDatabase& skillDB = ecs.getResource<Game::Combat::Skill::Data::SkillDatabase>();

	//for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	//{
	//	auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(e);


	//	// (状態レイヤー導入時変更予定 時間による制御はおかしいため)初回発動時のみ
	//	if (skillInstance.timeSinceCast > 0.0f)
	//		continue;


	//	std::cout << "[SkillCastingSystem] Checking if SkillID " << skillInstance.skillId << " exists..." << std::endl;
	//	if (!skillDB.Has(skillInstance.skillId)) {
	//		std::cout << "[SkillCastingSystem] SkillID not found, skipping." << std::endl;
	//		continue;
	//	}



	//	if (!skillDB.Has(skillInstance.skillId)) continue;
	//	const Game::Combat::Skill::Data::SkillDefinition& def = skillDB.Get(skillInstance.skillId);

	//	auto& transform = ecs.get<Engine::ECS::Component::Logic2D::Transform2DComponent>(e);
	//	const auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(skillInstance.caster);

	//	// world 変換
	//	glm::vec2 worldCenter = logic.positionXZ;
	//	Game::Combat::Skill::Component::Attack2DShape shape = def.attackSpec.shape;


	//	// 攻撃範囲エンティティ生成
	//	Engine::ECS::Entity attack = ecs.createEntity();
	//	Game::Combat::Skill::Component::Attack2DAreaComponent area;
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
	//	Game::Combat::Skill::Component::SkillTrajectoryComponent traj;
	//	traj.type = def.attackSpec.trajectoryType;
	//	traj.elapsedTime = 0.0f;
	//	traj.trajectoryFunc = Game::Combat::SkillFactory::SkillTrajectoryFactory::Create(def, transform);
	//	ecs.addComponent(attack, traj);
	//}
	//// std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] Hit Area is created successfully\n";

}

// 攻撃判定形状を作成(昔仕様：ECSグローバルリソース実装後廃止予定)
void Game::Combat::Skill::System::SpawnSkillHitArea(Engine::ECS::EntityMgr& ecs, Game::Combat::Skill::Data::SkillDatabase& skillDB)
{
	//for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::SkillInstanceComponent>())
	//{
	//	auto& skillInstance = ecs.get<Game::Combat::Skill::Component::SkillInstanceComponent>(e);


	//	// 初回発動時のみ
	//	if (skillInstance.timeSinceCast > 0.0f)
	//		continue;

	//	std::cout << "[SkillCastingSystem] skillId: " << skillInstance.skillId << std::endl;

	//	if (!skillDB.Has(skillInstance.skillId)) continue;
	//	const Game::Combat::Skill::Data::SkillDefinition& def = skillDB.Get(skillInstance.skillId);
	//	auto& transform = ecs.get<Engine::ECS::Component::Logic2D::Transform2DComponent>(e);
	//	const auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(skillInstance.caster);

	//	// world 変換
	//	glm::vec2 worldCenter = logic.positionXZ;
	//	Game::Combat::Skill::Component::Attack2DShape shape = def.attackSpec.shape;


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
	//	Engine::ECS::Entity attack = ecs.createEntity();
	//	Game::Combat::Skill::Component::Attack2DAreaComponent area;
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
	//	Game::Combat::Skill::Component::SkillTrajectoryComponent traj;
	//	traj.type = def.attackSpec.trajectoryType;
	//	traj.elapsedTime = 0.0f;
	//	traj.trajectoryFunc = Game::Combat::SkillFactory::SkillTrajectoryFactory::Create(def, transform);
	//	ecs.addComponent(attack, traj);
	//}
}
