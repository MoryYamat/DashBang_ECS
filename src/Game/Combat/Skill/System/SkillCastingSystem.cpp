// 

#include "SkillCastingSystem.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"
#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDefinition.h"

#include "Game/Combat/Skill/Component/SkillTrajectoryComponent.h"
#include "Game/Combat/Skill/Factory/SkillTrajectoryFactory.h"

#include "Engine/Math/MathUtils.h"

#include "Engine/Debug/DebugUtils.h"

void Game::Combat::Skill::System::SpawnSkillHitArea(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB)
{
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<gNsSkillComp::SkillInstanceComponent>(e);


		// 初回発動時のみ
		if (skillInstance.timeSinceCast > 0.0f)
			continue;

		std::cout << "[SkillCastingSystem] skillId: " << skillInstance.skillId << std::endl;

		const gNsSkillData::SkillDefinition& def = skillDB.Get(skillInstance.skillId);
		auto& transform = ecs.get<eNsLogic2DComp::Transform2DComponent>(e);
		const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(skillInstance.caster);

		// world 変換
		glm::vec2 worldCenter = logic.positionXZ;
		gNsSkillComp::Attack2DShape shape = def.shape;

	
		// ローカル形状情報とワールド変換情報を分離したためこの部分の分岐は不要
		// Since the local shape information and the world transformation information have been separated, 
		// this branch is no longer necessary.
		//if (def.shape.IsCircle())
		//{
		//	// Local shape information
		//	Circle2DAttack circle = def.shape.AsCircle();
		//	shape = Attack2DShape{ circle };

		//	// Update the World Coordinate Information
		//	// glm::vec2 offset = MathUtils::RotateVec2(circle.localcenter, transform.rotationY) * transform.scale;
		//	// transform.positionXZ = transform.positionXZ + offset;
		//}
		//else if (def.shape.IsSector())
		//{
		//	// Local shape information
		//	Sector2DAttack sector = def.shape.AsSector();
		//	//glm::vec2 direct = transform.GetFrontXZ();
		//	//sector.localdirection = direct;
		//	shape = Attack2DShape{ sector };

		//	// Update the World Coordinate Information
		//	// glm::vec2 offset = MathUtils::RotateVec2(sector.localcenter, transform.rotationY) * transform.scale;
		//	//transform.positionXZ = transform.positionXZ + offset;

		//	// DebugUtils::LogVector_string("SkillCastingSystem.cpp(direction)", sector.localdirection);
		//}
		//else if (def.shape.IsRectangle())
		//{
		//	// Local shape information
		//	Rectangle2DAttack rect = def.shape.AsRectangle();
		//	// rect.localdirection = glm::normalize(MathUtils::RotateVec2(rect.localdirection, transform.rotationY));
		//	shape = Attack2DShape{ rect };

		//	// Update the World Coordinate Information
		//	// glm::vec2 offset = MathUtils::RotateVec2(rect.localcenter, transform.rotationY) * transform.scale;
		//	//transform.positionXZ = transform.positionXZ + offset;
		//}

		// 攻撃範囲エンティティ生成
		eNsECS::Entity attack = ecs.createEntity();
		gNsSkillComp::Attack2DAreaComponent area;
		area.shape = shape;
		area.lifetime = def.duration;
		area.owner = skillInstance.caster;
		area.skillEntity = e;
		std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] create: " << def.name << " is created. \n";
		ecs.addComponent(attack, area);

		skillInstance.spawnedHitAreas.push_back(attack);

		// transformを複製
		ecs.addComponent(attack, transform);

		// 攻撃判定 Entityに軌跡コンポーネントを追加
		gNsSkillComp::SkillTrajectoryComponent traj;
		traj.type = def.trajectoryType;
		traj.elapsedTime = 0.0f;
		traj.trajectoryFunc = gNsSkillFactory::SkillTrajectoryFactory::Create(def, transform);
		ecs.addComponent(attack, traj);
	}
}
