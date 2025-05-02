#include "Game/Actor/FollowCameraActor.h"

#include "Core/ECS/Entity.h"

#include "Core/ECS/Component/PlayerControllerComponent.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/FollowCameraComponent.h"
#include "Core/ECS/Component/CameraComponent.h"

#include "Core/Utils/EntityUtils.h"

#include <cstdint>
#include <iostream>

FollowCameraActor::FollowCameraActor(ECS& ecs)
{
	Entity entity = ecs.createEntity();

	CameraComponent camComp;

	FollowCameraComponent followCamComp;

	// Get the ID of the target entity to follow and set it to target
	Entity ePlayer = EntityUtils::getEntityByName(ecs, "Player");
	if (ePlayer != Entity::INVALID)
	{
		followCamComp.targetEntity = ePlayer.id;
		ecs.addComponent(entity, followCamComp);
		const auto& targetTransform = ecs.get<TransformComponent>(ePlayer);

		std::cout << "[FollowCameraActor.cpp]: targetTransform x. " << targetTransform.position.z << std::endl;

		//std::cout << "[FollowCameraActor.cpp]: Player's entity id is." << id << std::endl;
		//std::cout << "[PlayerCharacterActor.cpp]: The camera for the PlayerCharacter has been set up." << std::endl;
	}
	else
	{
		std::cerr << "[FollowCameraActor.cpp]: Failed to find Player entity by name!" << std::endl;
	}

	TransformComponent CamTransformComp;



	//CamTransformComp.position.x = 

}


void FollowCameraActor::Update(ECS& ecs, float deltaTime)
{

}


//// search entity id
//uint32_t id = EntityUtils::getEntityByName(ecs, nameComp.name);
//
//if (id != INVALID_ENTITY_ID)
//{
//	FollowCameraComponent cam;
//	cam.targetEntity = id;
//	ecs.addComponent(entity, cam);
//	//std::cout << "[PlayerCharacterActor.cpp]: The camera for the PlayerCharacter has been set up." << std::endl;
//}
//else
//{
//	std::cerr << "[PlayerCharacterActor.cpp]: Failed to find Player entity by name!" << std::endl;
//}