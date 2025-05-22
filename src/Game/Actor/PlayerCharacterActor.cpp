#include "PlayerCharacterActor.h"

#include "Core/ECS/Entity.h"
#include "Core/EntityUtils/EntityUtils.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"

#include "Core/ECS/Component/FollowCameraComponent.h"

// input
// #include "Core/ECS/Component/InputComponent.h"
#include "Core/ECS/Component/InputActionComponent.h"

// test color
#include "Core/ECS/Component/MaterialComponent.h"

// Flags
#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Core/ECS/Component/NameComponent.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"

// collision
#include "Core/ECS/Component/Collision/CollisionComponent.h"
//#include "Core/ECS/Component/Collision/ColliderType.h"

// Skill
#include "Game/SkillSystem/Component/SkillInstanceComponent.h"
#include "Game/SkillSystem/Component/ActiveSkillCasterComponent.h"
#include "Game/SkillSystem/Component/SkillSlotAssignmentComponent.h"

#include "DataTypes/ModelData.h"
#include "Graphics/Model/AssimpImporter.h"
#include "Graphics/Renderer/GPUBufferUtils.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/InitTileMap/InitTileMap.h"

#include "Debug/DebugUtils.h"

#include <iostream>

PlayerCharacter::PlayerCharacter(ECS& ecs, Shader* shader)
{
	Entity entity = ecs.createEntity();

	// load Model Datas from file
	ModelData modelData = AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// set Mesh data to GPUBuffers
	ModelGPU modelGPU = GPUBufferUtils::createMeshGPUBuffers(modelData);

	// set MeshComponent
	ecs.addComponent(entity, MeshComponent{
			modelData,
			modelGPU
		});


	// set TransformComponent
	TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	ecs.addComponent(entity, transformComp);


	// set ShaderComponent
	ShaderComponent shaderComp;
	shaderComp.shader = shader;
	if (shaderComp.shader)
	{
		shaderComp.shader->Use();
		shaderComp.shader->setMat4("model", transformComp.toMatrix());
		std::cout << "[PlayerCharacterActor.cpp]: The model matrix was set successfully." << std::endl;
	}
	else
	{
		std::cout << "[PlayerCharacterActor.cpp]: Shader not found." << std::endl;
	}
	ecs.addComponent(entity, shaderComp);

	// Input State Component
	//InputComponent input;
	//ecs.addComponent(entity, input);
	InputActionComponent inputActionComp;
	ecs.addComponent(entity, inputActionComp);

	// Logic2D
	Logic2DTransformComponent logic;
	logic = GameInit::LogicTransform::InitLogic2DTransformFromModel(transformComp, modelData);
	ecs.addComponent(entity, logic);

	// Controller Flag
	PlayerControllerComponent PCflag;
	ecs.addComponent(entity, PCflag);

	// Set NameComponent
	NameComponent nameComp;
	nameComp.name = "Player";
	ecs.addComponent(entity, nameComp);


	// set Test Corlor
	MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(0.8f, 0.4f, 0.2f);
	ecs.addComponent(entity, materialComp);

	// Collsion Initialization
	// コリジョン初期化
	CollisionComponent playerCollisionComp;
	
	playerCollisionComp.collider.type = ColliderType::Circle2D;
	playerCollisionComp.collider.circle2D.center = logic.positionXZ;
	playerCollisionComp.isStatic = false;
	float radius = GameInit::LogicTransform::EstimateRadiusFromModelXZ(transformComp, modelData, GameInit::LogicTransform::RadiusEstimateStrategy::MaxAxis);

	playerCollisionComp.collider.circle2D.radius = radius;
	ecs.addComponent(entity, playerCollisionComp);


	// 以前の設計
	//SkillInstanceComponent activeSkill1;
	//activeSkill1.caster = entity;
	//activeSkill1.timeSinceCast = 0.0f;
	//activeSkill1.skillId = 1;
	//ecs.addComponent(entity, activeSkill1);

	//SkillInstanceComponent activeSkill2;
	//activeSkill2.caster = entity;
	//activeSkill2.timeSinceCast = 0.0f;
	//activeSkill2.skillId = 2;
	//ecs.addComponent(entity, activeSkill2);

	// スキル（ID） を SkillSLotに割り当て
	SkillSlotAssignmentComponent assign;
	assign.slotToSkillId[SkillSystem::SkillSlot::Primary] = 1;
	assign.slotToSkillId[SkillSystem::SkillSlot::Secondary] = 2;
	assign.slotToSkillId[SkillSystem::SkillSlot::Utility1] = 3;
	ecs.addComponent(entity, assign);

	ActiveSkillCasterComponent ascc;
	ecs.addComponent(entity, ascc);

	std::cout << "[[PlayerCharacterActor.cpp(radius)] : radius. " << radius << std::endl;

	//std::cout << "[PlayerCharacterActor.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	//std::cout << "[PlayerCharacterActor.cpp]: Logic Rotation " << logic.rotation << std::endl;
	//std::cout << "[PlayerCharacterActor.cpp]: Logic Scale: x. " << logic.scale.x << " z. " << logic.scale.y << std::endl;

	std::cout << "[PlayerCharacterActor.cpp]: Test3Dmodel Settings Completed" << std::endl;

	DebugUtils::LogVector("PlayerCharacterActor.cpp(Color)", materialComp.baseColor);
}

PlayerCharacter::~PlayerCharacter()
{

}

