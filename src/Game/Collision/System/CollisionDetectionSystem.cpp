#include "CollisionDetectionSystem.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"
#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"
#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Collision/Extractor/PlayerTileCollisionExtractor.h"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include "Game/Collision/Data/CollisionContextData.h"

#include "Engine/Debug/DebugUtils.h"

#include <glm/glm.hpp>

#include <iostream>

void Game::Collision::System::UpdateCollisionResultStorage(eNsECS::EntityMgr& ecs, gNsCollData::CollisionResultStorage& collisionResultStorage)
{
	Game::Collision::Data::PlayerCollisionContext playerCollisionCtx;

	for (eNsECS::Entity e : ecs.view<gNsTags::PlayerCharacterTag, eNsLogic2DComp::Logic2DTransformComponent, eNsLogic2DComp::CollisionComponent>())
	{
		const auto& collisionComp = ecs.get<eNsLogic2DComp::CollisionComponent>(e);
		const auto& logic2DComp = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);

		// 更新 (名残，形状情報／変換情報の明確な責務分離後未使用)
		playerCollisionCtx.center = logic2DComp.positionXZ;
		// playerCollisionCtx.radius = collisionComp.collider.circle2D.radius;
	//	playerCollisionCtx.playerEntity = e;

		break;
	}

	eNsLogic2DComp::TileMapComponent tileMapComp;
	for (eNsECS::Entity e : ecs.view<eNsLogic2DComp::TileMapComponent>())
	{
		tileMapComp = ecs.get<eNsLogic2DComp::TileMapComponent>(e);

		std::vector<glm::ivec2> hitTileIndices = Game::Collision::Extract::ExtractPlayerTileCollisions(playerCollisionCtx, tileMapComp);

		collisionResultStorage.AddTileCollision(hitTileIndices);
	}
}




void Game::Collision::System::CollisionDetectionSystem(eNsECS::EntityMgr& ecs, gNsCollData::CollisionResultStorage& collisionResultStorage)
{

}



