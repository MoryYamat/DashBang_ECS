#include "TestObject.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityUtils/EntityUtils.h"
#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/ECS/Component/Graphics/MeshComponent.h"
#include "Engine/ECS/Component/Graphics/ShaderComponent.h"

#include "Engine/ECS/Component/Camera/FollowCameraComponent.h"

// input
// #include "Core/ECS/Component/InputComponent.h"
#include "Game/Input/Private/InputActionComponent.h"

// test color
#include "Engine/ECS/Component/Graphics/MaterialComponent.h"

// Flags
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Utils/NameComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Game/Collision/Public/CollisionMaskComponent.hpp"
#include "Game/ECS/Private/Component/TeamComponent.h"

#include "Engine/Graphics/Private/Renderer/Shader.h"

#include "Engine/Graphics/Private/Model/ModelData.h"
#include "Engine/Graphics/Private/Model/AssimpImporter.h"
#include "Engine/Graphics/Private/Renderer/GPUBufferUtils.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Game/Init/Private/InitModel/InitLogicTransformFromModel.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include <iostream>

Game::Actor::TestObject::TestObject(Engine::ECS::EntityMgr& ecs, Engine::Graphics::Shader* shader)
{
//	namespace Ops = Engine::ECS::Ops;
//	namespace Comp = Engine::ECS::Component;
//	namespace Model = Engine::Graphics::Model;
//	namespace Render = Engine::Graphics::Render;
//
//	Engine::ECS::Entity e = ecs.createEntity();
//
//	// load Model Datas from file
//	Engine::Graphics::Model::ModelData modelData = Model::AssimpImporter::Import("Assets/Models/Ch44_nonPBR.fbx");
//	for (const auto& mesh : modelData.meshes)
//	{
//		std::cout << "[PlayerCharacterActor.cpp]: Vertices: " << mesh.vertices.size()
//			<< ", Indices: " << mesh.indices.size()
//			<< ", hasIndices: " << mesh.hasIndices << std::endl;
//	}
//
//	// set Mesh data to GPUBuffers
//	Engine::Graphics::Model::ModelGPU modelGPU = Render::GPUBufferUtils::createMeshGPUBuffers(modelData);
//
//
//	// set TransformComponent
//	Comp::Common::TransformComponent transformComp;
//	transformComp.position = glm::vec3(0.0f, 0.0f, 0.0f);
//	transformComp.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
//	transformComp.scale = glm::vec3(0.01f);
//	Ops::Add<Comp::Common::TransformComponent>(ecs, e, transformComp);
//
//
//	// set ShaderComponent
//	Comp::Graphics::ShaderComponent shaderComp;
//	shaderComp.shader = shader;
//	if (shaderComp.shader)
//	{
//		shaderComp.shader->Use();
//		shaderComp.shader->setMat4("model", transformComp.toMatrix());
//		std::cout << "[PlayerCharacterActor.cpp]: The model matrix was set successfully." << std::endl;
//	}
//	else
//	{
//		std::cout << "[PlayerCharacterActor.cpp]: Shader not found." << std::endl;
//	}
//	Ops::Add<Comp::Graphics::ShaderComponent>(ecs, e, shaderComp);
//
//	// Logic2D
//	Comp::Logic2D::Logic2DTransformComponent logic;
//	logic = Game::Init::Logic2D::InitLogic2DTransformFromModel(transformComp, modelData);
//	Ops::Add<Comp::Logic2D::Logic2DTransformComponent>(ecs, e, logic);
//
//	// set Test Corlor
//	Comp::Graphics::MaterialComponent materialComp;
//	materialComp.baseColor = glm::vec3(1.0f, 1.0f, 1.0f);
//	Ops::Add<Comp::Graphics::MaterialComponent>(ecs, e, materialComp);
//
//	// Collsion Initialization
//// コリジョン初期化
//	Comp::Logic2D::CollisionComponent playerCollisionComp;
//	playerCollisionComp.collider.shape = Comp::Logic2D::Circle2D{
//		.center = glm::vec2(0.0f),// ローカルセンター
//		.radius = Game::Init::Logic2D::EstimateRadiusFromModelXZ(transformComp, modelData, Game::Init::Logic2D::RadiusEstimateStrategy::MaxAxis)
//	};
//	//playerCollisionComp.collider.circle2D.center = logic.positionXZ;
//	playerCollisionComp.isStatic = false;
//	// float radius = gNsInit::Logic2D::EstimateRadiusFromModelXZ(transformComp, modelData, gNsInit::Logic2D::RadiusEstimateStrategy::MaxAxis);
//	// playerCollisionComp.collider.circle2D.radius = radius;
//	Ops::Add<Comp::Logic2D::CollisionComponent>(ecs, e, playerCollisionComp);
//
//	// Collision Mask 初期化
//	Game::Collision::Component::CollisionMaskComponent testObjectMask;
//	testObjectMask = Game::Collision::Component::CollisionMaskPresets::Character();
//	Ops::Add<Game::Collision::Component::CollisionMaskComponent>(ecs, e, testObjectMask);
//
//	Ops::Add<Game::ECS::Component::TeamComponent>(ecs, e, Game::ECS::Component::TeamComponent{ .team =
//		Game::ECS::Component::Team::EnemyTeam });
//
//	// set MeshComponent
//	Ops::Add<Comp::Graphics::MeshComponent>(ecs, e, Comp::Graphics::MeshComponent{ std::move(modelData), std::move(modelGPU) });
//

}