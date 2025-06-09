#include "PlayerCharacterActor.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityUtils/EntityUtils.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"

#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

// input
// #include "Core/ECS/Component/InputComponent.h"
#include "Game/Input/InputActionComponent.h"

// Intent
#include "Game/Character/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/Movement/Component/Intent/FacingIntentComponent.h"
#include "Game/Character/Stats/Component/CharacterStatsComponent.h"

// test color
#include "Engine/ECS/Component/Graphics/MaterialComponent.h"

// Flags
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Utils/NameComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

// collision
#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
//#include "Core/ECS/Component/Collision/ColliderType.h"

// collision comp
#include "Game/Collision/Component/CollisionMaskComponent.h"

// Skill
#include "Game/Combat/Skill/Component/SkillInstanceComponent.h"
#include "Game/Combat/Skill/Component/ActiveSkillCasterComponent.h"
#include "Game/Combat/Skill/Component/SkillSlotAssignmentComponent.h"

#include "Game/Combat/Skill/Component/SkillInputBindingComponent.h"
// intent
#include "Game/Combat/Skill/Intent/Component/SkillIntentComponent.h"

#include "Engine/Graphics/Model/ModelData.h"
#include "Engine/Graphics/Model/AssimpImporter.h"
#include "Engine/Graphics/Renderer/GPUBufferUtils.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/InitTileMap/InitTileMap.h"



#include "Engine/Debug/DebugUtils.h"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

Game::Actor::Player::PlayerCharacter::PlayerCharacter(eNsECS::EntityMgr& ecs, eNsGfxRender::Shader* shader)
{
	eNsECS::Entity entity = ecs.createEntity();

	// load Model Datas from file
	eNsGfxModel::ModelData modelData = eNsGfxModel::AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}

	// set Mesh data to GPUBuffers
	eNsGfxModel::ModelGPU modelGPU = eNsGfxRender::GPUBufferUtils::createMeshGPUBuffers(modelData);

	// set MeshComponent
	ecs.addComponent(entity, eNsGfxComp::MeshComponent{
			modelData,
			modelGPU
		});


	// set TransformComponent
	eNsCommonComp::TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	ecs.addComponent(entity, transformComp);


	// set ShaderComponent
	eNsGfxComp::ShaderComponent shaderComp;
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
	gNsInput::InputActionComponent inputActionComp;
	ecs.addComponent(entity, inputActionComp);

	// Logic2D
	eNsLogic2DComp::Logic2DTransformComponent logic;
	logic = gNsInit::Logic2D::InitLogic2DTransformFromModel(transformComp, modelData);
	ecs.addComponent(entity, logic);

	// Controller Flag
	eNsTagComp::PlayerControllerComponent PCflag;
	ecs.addComponent(entity, PCflag);

	// Set NameComponent
	eNsUtilComp::NameComponent nameComp;
	nameComp.name = "Player";
	ecs.addComponent(entity, nameComp);


	// set Test Corlor
	eNsGfxComp::MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(0.8f, 0.4f, 0.2f);
	ecs.addComponent(entity, materialComp);

	// Collsion Initialization
	// コリジョン初期化
	eNsLogic2DComp::CollisionComponent playerCollisionComp;
	
	playerCollisionComp.collider.type = eNsLogic2DComp::ColliderType::Circle2D;
	playerCollisionComp.collider.circle2D.center = logic.positionXZ;
	playerCollisionComp.isStatic = false;
	float radius = gNsInit::Logic2D::EstimateRadiusFromModelXZ(transformComp, modelData, gNsInit::Logic2D::RadiusEstimateStrategy::MaxAxis);

	playerCollisionComp.collider.circle2D.radius = radius;
	ecs.addComponent(entity, playerCollisionComp);

	// Collision Mask 初期化
	gNsCollComp::CollisionMaskComponent playerMask;
	playerMask.selfLayer = gNsCollData::Layer::Player;
	playerMask.collidesWithMask = static_cast<uint8_t>(gNsCollData::Layer::Tile | gNsCollData::Layer::Tile);

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

	// スキル（ID） を SkillSLotに割り当て 情報を保持
	gNsSkillComp::SkillSlotAssignmentComponent assign;
	assign.slotToSkillId[gNsSkillData::SkillSlot::Primary] = 1;
	assign.slotToSkillId[gNsSkillData::SkillSlot::Secondary] = 2;
	assign.slotToSkillId[gNsSkillData::SkillSlot::Utility1] = 3;
	ecs.addComponent(entity, assign);

	// InputAction と スキルスロットの割り当てを保持
	gNsSkillComp::SkillInputBindingComponent binding;
	binding.actionToSlot[gNsInput::InputAction::CastSkill1] = gNsSkillData::SkillSlot::Primary;
	binding.actionToSlot[gNsInput::InputAction::CastSkill2] = gNsSkillData::SkillSlot::Secondary;
	binding.actionToSlot[gNsInput::InputAction::CastSkill3] = gNsSkillData::SkillSlot::Utility1;
	ecs.addComponent(entity, binding);

	// 必要？（未使用のため検討が必要）何のために用意したか不明
	ActiveSkillCasterComponent ascc;
	ecs.addComponent(entity, ascc);

	std::cout << "[[PlayerCharacterActor.cpp(radius)] : radius. " << radius << std::endl;

	//std::cout << "[PlayerCharacterActor.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	//std::cout << "[PlayerCharacterActor.cpp]: Logic Rotation " << logic.rotation << std::endl;
	//std::cout << "[PlayerCharacterActor.cpp]: Logic Scale: x. " << logic.scale.x << " z. " << logic.scale.y << std::endl;

	std::cout << "[PlayerCharacterActor.cpp]: Test3Dmodel Settings Completed" << std::endl;

	eNsDebugLog::LogVector("PlayerCharacterActor.cpp(Color)", materialComp.baseColor);

	// velocity
	ecs.addComponent(entity, eNsLogic2DComp::Velocity2DComponent{});

	// Intent base
	ecs.addComponent(entity, gNsSkillIntent::SkillIntentComponent{});

	// movement
	ecs.addComponent(entity, gNsCharacterIntent::MovementIntentComponent{});

	// front
	ecs.addComponent(entity, gNsCharacterIntent::FacingIntentComponent{});

	// stats
	ecs.addComponent(entity, gNsCharacter::Stats::CharacterStatsComponent{
		.moveSpeed = 5.0f
		});

	// Tag
	ecs.addComponent(entity, eNsTagComp::PlayerCharacterTag{});
}

Game::Actor::Player::PlayerCharacter::~PlayerCharacter()
{

}

