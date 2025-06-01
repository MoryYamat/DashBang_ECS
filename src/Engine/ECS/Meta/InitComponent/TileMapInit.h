// Special template for TileMapComponent initialization
// TileMapComponent初期化用の特殊テンプレート

#pragma once

#include "Engine/ECS/Meta/InitComponent/InitSystem.h"

#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Game/Init/InitTileMap/InitTileMap.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Engine::ECS::Meta::Init
{
	template<>
	struct InitSystem<eNsLogic2DComp::TileMapComponent>
	{
		static void Init(eNsLogic2DComp::TileMapComponent& tileMapComp, eNsECS::EntityMgr& ecs, eNsECS::Entity tileMapEntity, const eNsWindow::Window& window)
		{
			gNsInit::Logic2D::ApplyObstacleCollidersToTileMap(ecs, tileMapComp);
			gNsInit::Logic2D::MaskUncoveredTilesByTerrainOBB(ecs, tileMapComp);
		}

	};
}
