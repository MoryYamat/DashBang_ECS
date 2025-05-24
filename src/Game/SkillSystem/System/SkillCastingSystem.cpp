// 

#include "SkillCastingSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/Transform2DComponent.h"
#include "Game/SkillSystem/Component/SkillInstanceComponent.h"

#include "Game/SkillSystem/MasterData/SkillDefinition.h"

#include "Math/MathUtils.h"

#include "Debug/DebugUtils.h"

void SkillSystem::Casting::SpawnSkillHitArea(ECS& ecs, SkillDatabase& skillDB)
{
	for (Entity e : ecs.view<SkillInstanceComponent>())
	{
		auto& skillInstance = ecs.get<SkillInstanceComponent>(e);


		// 初回発動時のみ
		if (skillInstance.timeSinceCast > 0.0f)
			continue;

		std::cout << "[SkillCastingSystem] skillId: " << skillInstance.skillId << std::endl;

		const SkillDefinition& def = skillDB.Get(skillInstance.skillId);
		auto& transform = ecs.get<Transform2DComponent>(e);
		const auto& logic = ecs.get<Logic2DTransformComponent>(skillInstance.caster);

		// world 変換
		glm::vec2 worldCenter = logic.positionXZ;
		Attack2DShape shape = def.shape;

	
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
		Entity attack = ecs.createEntity();
		Attack2DAreaComponent area;
		area.shape = shape;
		area.lifetime = def.duration;
		area.owner = skillInstance.caster;
		area.skillEntity = e;
		std::cout << "[SkillCastingSystem.cpp(SpawnSkillHitArea)] create: " << def.name << " is created. \n";
		ecs.addComponent(attack, area);

		// transformを複製
		ecs.addComponent(attack, transform);
	}
}
