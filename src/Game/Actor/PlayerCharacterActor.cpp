#include "PlayerCharacterActor.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityUtils/EntityUtils.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"

// shader
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"
#include "Engine/ECS/Component/Graphics/AnimatorComponent.hpp"


#include "Engine/ECS/Component/Logic2D/Velocity2DComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

#include "Engine/Graphics/Private/Renderer/Shader.h"

// input
// #include "Core/ECS/Component/InputComponent.h"
#include "Game/Input/Private/InputActionComponent.h"

// Intent
#include "Game/Character/Private/Control/Movement/Component/Intent/MovementIntentComponent.h"
#include "Game/Character/Private/Control/Movement/Component/Intent/FacingIntentComponent.h"
#include "Game/Character/Private/Stats/Public/StatsComponent.hpp"

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
#include "Game/Collision/Private/Component/CollisionMaskComponent.h"

// Skill
#include "Game/Combat/Private/Skill/Component/SkillInstanceComponent.h"
#include "Game/Combat/Private/Skill/Component/ActiveSkillCasterComponent.h"
#include "Game/Combat/Private/Skill/Component/SkillSlotAssignmentComponent.h"

#include "Game/Combat/Private/Skill/Component/SkillInputBindingComponent.h"

#include "Game/Combat/Private/Skill/Context/Component/SkillExecutionContextComponent.hpp"


// intent
#include "Game/Character/Private/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Game/Character/Private/State/Component/Action/Skill/CharacterSkillExecutionStateComponent.hpp"

#include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Graphics/Private/Model/AssimpImporter.h"
#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"

#include "Engine/Graphics/Private/Model/CgltfImporter.hpp"


#include "Game/Init/Private/InitModel/InitLogicTransformFromModel.h"
#include "Game/Init/Private/InitTileMap/InitTileMap.h"



// character 

// animation
#include "Game/Character/Private/Animation/Query/Locomotion/LocomotionAnimationQueryComponent.hpp"
#include "Game/Character/Private/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"
#include "Game/Character/Private/Animation/Profile/AnimationProfileComponent.hpp"
#include "Game/Character/Private/Animation/Arbiter/FinalAnimationDecisionComponent.hpp"

#include "Game/Character/Private/Animation/Query/Skill/SkillAnimationQueryComponent.hpp"
#include "Game/Character/Private/Animation/Resolve/Skill/SkillAnimDecisionComponent.hpp"

#include "Game/Character/Private/Animation/Query/CC/CCAnimationQueryComponent.hpp"
#include "Game/Character/Private/Animation/Resolve/CC/CCAnimDecisionComponent.hpp"

// life
#include "Game/Character/Private/State/Component/LifeStateComponent.hpp"
// movement
#include "Game/Character/Private/State/Component/MovementStateComponent.hpp"
// action
#include "Game/Character/Private/State/Component/Action/ActionStateComponent.hpp"

// =========== FSM ===========
// 
// movement
#include "Game/Character/Private/FSM/Movement/StateModel/MovementStateComponent.hpp"
#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMTransitionRequestComponent.hpp"
#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMLeaseComponent.hpp"
#include "Game/Character/Private/FSM/Movement/StateModel/MovementFSMInterferenceRequestComponent.hpp"
//CC
#include "Game/Character/Private/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCFSMTransitionRequestComponent.hpp"
#include "Game/Character/Private/FSM/CC/StateModel/CCDedupStampComponent.hpp"

#include "Game/Character/Private/Control/CC/Component/CCAntiChainComponent.hpp"

// skill
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillStateComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMTransitionRequestComponent.hpp"
#include "Game/Combat/Private/Skill/Component/SkillExecutionContextComponent.hpp"
#include "Game/Combat/Private/Skill/Component/SkillEffectExecutionRecordComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMInterferenceRequestComponent.hpp"
#include "Game/Combat/Private/Skill/FSM/StateModel/SkillFSMLeaseComponent.hpp"

// CC

// Game ECS
#include "Game/ECS/Private/Tags/CharacterAttribTags.h"
#include "Game/ECS/Private/Component/TeamComponent.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <iostream>

Game::Actor::Player::PlayerCharacter::PlayerCharacter(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Render::Shader* shader)
{
	namespace Ops = Engine::ECS::Ops;
	namespace Comp = Engine::ECS::Component;

	Engine::ECS::Entity e = ecs.createEntity();

	// load Model Datas from file
	// Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
	// =============================== test ==================================
	// Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::CgltfImporter::Import("Assets/Models/paladdin_w_prop.glb");
	// Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::CgltfImporter::Import("Assets/Models/paladin/paladin_run_idle_default.glb");
	// Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::CgltfImporter::Import("Assets/Models/paladin/base_action_animation.glb");// run_fwd_default/run_back/run_right/run_left/rolling_fwd_default
	Engine::Graphics::Model::ModelData modelData = Engine::Graphics::Model::CgltfImporter::Import("Assets/Models/paladin/base_action_animation_diago.glb");// run_fwd_default/run_back/run_right/run_left/rolling_fwd_default
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
			<< ", Indices: " << mesh.indices.size()
			<< ", hasIndices: " << mesh.hasIndices << std::endl;
	}


	// set Mesh data to GPUBuffers
	Engine::Graphics::Model::ModelGPU modelGPU = Engine::Graphics::Render::GPUBufferUtils::createMeshGPUBuffers(modelData);

	// move 前に 必要な情報を設定
	// set TransformComponent
	Comp::Common::TransformComponent transformComp;
	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	transformComp.scale = glm::vec3(1.0f);
	Ops::Add<Comp::Common::TransformComponent>(ecs, e, transformComp);

	// Logic2D
	Comp::Logic2D::Logic2DTransformComponent logic;
	logic = Game::Init::Logic2D::InitLogic2DTransformFromModel(transformComp, modelData);
	Ops::Add<Comp::Logic2D::Logic2DTransformComponent>(ecs, e, logic);

	// animator comp // mesh-> move 前
	auto& anim = Ops::Add<Comp::Graphics::AnimatorComponent>(ecs, e,
		Comp::Graphics::AnimatorComponent{});

	// skill animation
	Engine::Graphics::Model::CgltfImporter::ImportAnimationsInto("Assets/Models/paladin/test_slash_outward_trim.glb", modelData);
	
	// cc animation
	Engine::Graphics::Model::CgltfImporter::ImportAnimationsInto("Assets/Models/paladin/stunned_default.glb", modelData);
	Engine::Graphics::Model::CgltfImporter::ImportAnimationsInto("Assets/Models/paladin/falling_back_default.glb", modelData);

	//const auto& clips = modelData.clips;
	//if (!clips.empty())
	//{
	//	if (!Comp::Graphics::SetClipByName(modelData, anim, "idle_default", true))
	//	{
	//		anim.clipIndex = 0;
	//		anim.time = 0.f;
	//		anim.speed = 1.f;
	//		anim.loop = true;
	//	}
	//	std::cout << "[PlayerCharacterActor] Playing clip: "
	//		<< clips[(size_t)anim.clipIndex].name
	//		<< " (dur=" << clips[(size_t)anim.clipIndex].duration
	//		<< "s, channels=" << clips[(size_t)anim.clipIndex].channels.size()
	//		<< ")\n";
	//}
	//else
	//{
	//	std::cout << "[PlayerCharacterActor] No animation clips in model.\n";
	//}

	// Collsion Initialization
	// コリジョン初期化
	Comp::Logic2D::CollisionComponent playerCollisionComp;
	playerCollisionComp.collider.shape = Engine::ECS::Component::Logic2D::Circle2D{
		.center = glm::vec2(0.0f),// ローカルセンター
		.radius = Game::Init::Logic2D::EstimateRadiusFromModelXZ(transformComp, modelData, Game::Init::Logic2D::RadiusEstimateStrategy::MaxAxis)
	};
	//playerCollisionComp.collider.circle2D.center = logic.positionXZ;
	playerCollisionComp.isStatic = false;
	// float radius = gNsInit::Logic2D::EstimateRadiusFromModelXZ(transformComp, modelData, gNsInit::Logic2D::RadiusEstimateStrategy::MaxAxis);
	// playerCollisionComp.collider.circle2D.radius = radius;
	Ops::Add<Comp::Logic2D::CollisionComponent>(ecs, e, playerCollisionComp);


	// set MeshComponent
	Ops::Add<Comp::Graphics::MeshComponent>(ecs, e,
		Comp::Graphics::MeshComponent
		{
			std::move(modelData),
			std::move(modelGPU)
		});




	// set ShaderComponent
	Comp::Graphics::ShaderComponent shaderComp;
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
	Ops::Add<Comp::Graphics::ShaderComponent>(ecs, e, shader);

	// Input State Component
	//InputComponent input;
	//ecs.addComponent(entity, input);
	Game::Input::InputActionComponent inputActionComp{};
	Ops::Add<Game::Input::InputActionComponent>(ecs, e, inputActionComp);



	// Controller Flag
	Comp::Tags::PlayerControllerComponent PCflag;
	Ops::Add<Comp::Tags::PlayerControllerComponent>(ecs, e, PCflag);

	// Set NameComponent
	Comp::Utils::NameComponent nameComp;
	nameComp.name = "Player";
	Ops::Add<Comp::Utils::NameComponent>(ecs, e, nameComp);


	// set Test Corlor
	Comp::Graphics::MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(0.8f, 0.4f, 0.2f);
	Ops::Add<Comp::Graphics::MaterialComponent>(ecs, e, materialComp);



	// Collision Mask 初期化
	Game::Collision::Component::CollisionMaskComponent playerMask;
	playerMask = Game::Collision::Component::CollisionMaskPresets::Character();
	Ops::Add<Game::Collision::Component::CollisionMaskComponent>(ecs, e, playerMask);

	Ops::Add<Game::ECS::Component::TeamComponent>
		(ecs, e, Game::ECS::Component::TeamComponent{ .team = Game::ECS::Component::Team::PlayerTeam });

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
	namespace Skill = Game::Combat::Skill;
	Skill::Component::SkillSlotAssignmentComponent assign;
	assign.slotToSkillId[Skill::Data::SkillSlot::Primary] = 1;
	assign.slotToSkillId[Skill::Data::SkillSlot::Secondary] = 2;
	assign.slotToSkillId[Skill::Data::SkillSlot::Utility1] = 3;
	assign.slotToSkillId[Skill::Data::SkillSlot::Utility2] = 4;
	Ops::Add<Game::Combat::Skill::Component::SkillSlotAssignmentComponent>(ecs, e, assign);

	// InputAction と スキルスロットの割り当てを保持
	namespace GameInput = Game::Input;
	Skill::Component::SkillInputBindingComponent binding;
	binding.actionToSlot[GameInput::InputAction::CastSkill1] = Skill::Data::SkillSlot::Primary;
	binding.actionToSlot[GameInput::InputAction::CastSkill2] = Skill::Data::SkillSlot::Secondary;
	binding.actionToSlot[GameInput::InputAction::CastSkill3] = Skill::Data::SkillSlot::Utility1;
	binding.actionToSlot[GameInput::InputAction::CastSkill4] = Skill::Data::SkillSlot::Utility2;
	Ops::Add<Game::Combat::Skill::Component::SkillInputBindingComponent>(ecs, e, binding);


	// Skill Context
	//Ops::Add<Game::Combat::Skill::Component::SkillExecutionContextComponent>(ecs, e,
	//	Game::Combat::Skill::Component::SkillExecutionContextComponent{ .caster = e }
	//);
	// std::cout << "[[PlayerCharacterActor.cpp(radius)] : radius. " << radius << std::endl;

	//std::cout << "[PlayerCharacterActor.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	//std::cout << "[PlayerCharacterActor.cpp]: Logic Rotation " << logic.rotation << std::endl;
	//std::cout << "[PlayerCharacterActor.cpp]: Logic Scale: x. " << logic.scale.x << " z. " << logic.scale.y << std::endl;

	std::cout << "[PlayerCharacterActor.cpp]: Test3Dmodel Settings Completed" << std::endl;

	Engine::Debug::Logging::LogVector("PlayerCharacterActor.cpp(Color)", materialComp.baseColor);

	// velocity
	Ops::Add<Comp::Logic2D::Velocity2DComponent>(ecs, e, Comp::Logic2D::Velocity2DComponent{});

	// Intent base
	Ops::Add<Game::Character::Control::Skill::SkillIntentComponent>(ecs, e, Game::Character::Control::Skill::SkillIntentComponent{});

	// movement
	Ops::Add<Game::Character::Intent::MovementIntentComponent>(ecs, e, Game::Character::Intent::MovementIntentComponent{});

	// front
	Ops::Add<Game::Character::Intent::FacingIntentComponent>(ecs, e, Game::Character::Intent::FacingIntentComponent{});

	// stats
	Ops::Add<Game::Character::Stats::CharacterStatsComponent>(ecs, e, Game::Character::Stats::CharacterStatsComponent{ .moveSpeed = 5.0f });
	

	// Tag
	Ops::Add<Game::ECS::Tags::PlayerCharacterTag>(ecs, e, Game::ECS::Tags::PlayerCharacterTag{});




	// logical state comp
	// life
	//ecs.addComponent(entity, Game::Character::State::Life::CharacterLifeStateComponent{});

	//// movenet
	//ecs.addComponent(entity, Game::Character::State::Movement::CharacterMovementStateComponent{});

	//// action
	//ecs.addComponent(entity, Game::Character::State::Action::CharacterActionStateComponent{});

	//// skill execution state
	//ecs.addComponent(entity, Game::Character::State::Action::CharacterSkillExecutionStateComponent{});





	// =========== FSM ===========
	// interference
	// movement FSM
	Ops::Add<Game::Character::FSM::Movement::MovementStateComponent>(ecs, e, Game::Character::FSM::Movement::MovementStateComponent{});
	Ops::Add<Game::Character::FSM::Movement::MovementFSMTransitionRequestComponent>(ecs, e, Game::Character::FSM::Movement::MovementFSMTransitionRequestComponent{});
	Ops::Add<Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent>(ecs, e, Game::Character::FSM::Movement::StateModel::MovementFSMLeaseComponent{});
	Ops::Add<Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent>(ecs, e, Game::Character::FSM::Movement::StateModel::MovementFSMInterferenceRequestComponent{});

	// CC FSM
	namespace CC = Game::Character::FSM::CC;
	Ops::Add<CC::StateModel::CCStateComponent>(ecs, e, CC::StateModel::CCStateComponent{});
	Ops::Add<CC::StateModel::CCFSMTransitionRequestComponent>(ecs, e, CC::StateModel::CCFSMTransitionRequestComponent{});
	Ops::Add<Game::Character::Control::CC::Component::CCAntiChainComponent>(ecs, e, Game::Character::Control::CC::Component::CCAntiChainComponent{});
	Ops::Add<CC::StateModel::CCDedupStampComponent>(ecs, e,  CC::StateModel::CCDedupStampComponent{});

	// skill FSM
	// namespace alias
	namespace SkillFSM = Game::Combat::Skill::FSM::StateModel;
	// 
	Ops::Add<SkillFSM::SkillStateComponent>(ecs, e, SkillFSM::SkillStateComponent{});
	Ops::Add<SkillFSM::SkillFSMTransitionRequestComponent>(ecs, e, SkillFSM::SkillFSMTransitionRequestComponent{});
	Ops::Add<Game::Combat::Skill::Component::SkillExecutionContextComponent>(ecs, e, Game::Combat::Skill::Component::SkillExecutionContextComponent{ .caster = e,
		});
	Ops::Add<Game::Combat::Skill::Component::SkillEffectExecutionRecordComponent>(ecs, e, Game::Combat::Skill::Component::SkillEffectExecutionRecordComponent{});
	Ops::Add<SkillFSM::SkillFSMInterferenceRequestComponent>(ecs, e, SkillFSM::SkillFSMInterferenceRequestComponent{});
	Ops::Add<SkillFSM::SkillFSMLeaseComponent>(ecs, e, SkillFSM::SkillFSMLeaseComponent{});
	// std::cout << "[PlayerCharacterActor.cpp] Created Player Entity: " << entity.id << std::endl;
	//if (ecs.hasComponent<gNsCharacterState::CharacterStateComponent>(entity)) {
	//	std::cout << "[確認] CharacterStateComponent は Entity " << entity.id << " に存在しています" << std::endl;
	//}
	//else {
	//	std::cout << "[警告] CharacterStateComponent は Entity " << entity.id << " に存在していません" << std::endl;
	//}

	//std::cout << "addComponent の typeid: " << typeid(gNsCharacterState::CharacterStateComponent).name() << std::endl;

	//for (Engine::ECS::Entity e : ecs.view<gNsCharacterState::CharacterStateComponent>()) {
	//	std::cout << "view の Entity: " << e.id << std::endl;
	//}
	//std::cout << "view の typeid: " << typeid(decltype(ecs.view<gNsCharacterState::CharacterStateComponent>())).name() << std::endl;


	// Animation
	// Movement
	namespace AnimQuery = Game::Character::Animation::Query;
	namespace AnimDec = Game::Character::Animation::Resolve;
	namespace AnimProf = Game::Character::Animation::Profile;
	namespace AnimFinal = Game::Character::Animation::Arbiter;
	Ops::Add<AnimQuery::LocomotionAnimQueryComponent>(ecs, e, AnimQuery::LocomotionAnimQueryComponent{});
	Ops::Add<AnimDec::Movement::MovementAnimDecisionComponent>(ecs, e, AnimDec::Movement::MovementAnimDecisionComponent{});
	Ops::Add<AnimProf::AnimationProfileComponent>(ecs, e, AnimProf::AnimationProfileComponent{ .profileId = "PaladinDefault" });
	Ops::Add<AnimFinal::FinalAnimationDecisionComponent>(ecs, e, AnimFinal::FinalAnimationDecisionComponent{});

	// skill
	Ops::Add<AnimQuery::SkillAnimQueryComponent>(ecs, e, AnimQuery::SkillAnimQueryComponent{});
	Ops::Add<AnimDec::Skill::SkillAnimDecisionComponent>(ecs, e, AnimDec::Skill::SkillAnimDecisionComponent{});

	// CC
	Ops::Add<AnimQuery::CC::CCAnimationQueryComponent>(ecs, e, AnimQuery::CC::CCAnimationQueryComponent{});
	Ops::Add<AnimDec::CC::CCAnimDecisionComponent>(ecs, e, AnimDec::CC::CCAnimDecisionComponent{});
}

Game::Actor::Player::PlayerCharacter::~PlayerCharacter()
{

}

