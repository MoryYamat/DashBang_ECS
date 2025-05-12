#include "TestRockActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/MeshComponent.h"
#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/ShaderComponent.h"
#include "Core/ECS/Component/MaterialComponent.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Core/ECS/Component/Collision/CollisionComponent.h"

#include "Core/ECS/Component/Tags/ObstacleTagComponent.h"

#include "DataTypes/ModelData.h"

#include "Game/Init/InitModel/InitLogicTransformFromModel.h"

#include "Graphics/Model/AssimpImporter.h"
#include "Graphics/Renderer/GPUBufferUtils.h"

#include "Debug/DebugUtils.h"

#include <iostream>

TestRockActor::TestRockActor(ECS& ecs, Shader* shader)
{
	Entity entity = ecs.createEntity();

	ModelData modelData = AssimpImporter::Import("Assets/Models/TestRock.fbx");
	for (const auto& mesh : modelData.meshes)
	{
		std::cout << "[TestRockActor.cpp]: Vertices: " << mesh.vertices.size()
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
	transformComp.position = glm::vec3(-10.0f, 0.0f, 2.0f);
	transformComp.rotation = glm::vec3(0.0f, 30.0f, 0.0f);
	transformComp.scale = glm::vec3(0.01f);
	ecs.addComponent(entity, transformComp);

	// set ShaderComponent
	ShaderComponent shaderComp;
	shaderComp.shader = shader;
	if (shaderComp.shader)
	{
		shaderComp.shader->Use();
		shaderComp.shader->setMat4("model", transformComp.toMatrix());
		std::cout << "[TestRockActor.cpp]: The model matrix was set successfully." << std::endl;
	}
	else
	{
		std::cout << "[TestRockActor.cpp]: Shader not found." << std::endl;
	}
	ecs.addComponent(entity, shaderComp);


	// Logic2D
	Logic2DTransformComponent logic;
	logic = GameInit::LogicTransform::InitLogic2DTransformFromModel(transformComp, modelData);
	ecs.addComponent(entity, logic);


	// set Test Corlor
	MaterialComponent materialComp;
	materialComp.baseColor = glm::vec3(1.0f, 0.0f, 1.0f);
	ecs.addComponent(entity, materialComp);

	// for collision setting
	CollisionComponent testRockCollisionComp;
	testRockCollisionComp.collider.type = ColliderType::Obb2D;
	// calc world size on the xz plane
	glm::vec2 obbSize = GameInit::LogicTransform::GetModelXZSizeWithScale(transformComp, modelData);
	testRockCollisionComp.collider.obb2D.halfExtents = obbSize / 2.0f;

	// calc world center on the xz plane
	glm::vec3 localCenter = modelData.GetCenter();
	glm::vec3 worldCenter3D = transformComp.toMatrix() * glm::vec4(localCenter, 1.0f);
	glm::vec2 worldCenterXZ = glm::vec2(worldCenter3D.x, worldCenter3D.z);
	testRockCollisionComp.collider.obb2D.center = worldCenterXZ;

	// calc local vector axisX and axisZ
	float rotRad = glm::radians(logic.rotation);
	glm::vec2 axisX = glm::normalize(glm::vec2(std::cos(rotRad), std::sin(rotRad)));
	glm::vec2 axisZ = glm::vec2(-axisX.y, axisX.x);
	testRockCollisionComp.collider.obb2D.axisX = axisX;
	testRockCollisionComp.collider.obb2D.axisZ = axisZ;
	ecs.addComponent(entity, testRockCollisionComp);
	
	// obstacle object's tag
	ObstacleTagComponent obstacleTagComp;
	ecs.addComponent(entity, obstacleTagComp);

	// Log
	DebugUtils::LogVector_string("[TestRockActor.cpp(CollsionCenter)]: ", testRockCollisionComp.collider.obb2D.center);
	DebugUtils::LogVector_string("[TestRockActor.cpp(AxisX)]: ", testRockCollisionComp.collider.obb2D.axisX);
	DebugUtils::LogVector_string("[TestRockActor.cpp(AxisZ)]: ", testRockCollisionComp.collider.obb2D.axisZ);



	std::cout << "[TestRockActor.cpp]: Test3Dmodel Settings Completed" << std::endl;
}