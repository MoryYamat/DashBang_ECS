// Class that creates TileMapEntity

#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"
#include "Engine/Graphics/Model/ModelData.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Actor::Map
{
	// (現在未使用)テストタイルマップ用(削除予定)
	// (現在未使用)テストタイルマップ用(削除予定)
	// (現在未使用)テストタイルマップ用(削除予定)
	class TileMapActor
	{
	public:

		// TerrainMeshから呼び出す (将来的には動的生成／削除およびLODに対応予定)
		static eNsECS::Entity Create(
			eNsECS::EntityMgr& ecs,
			const eNsCommonComp::TransformComponent& transform,
			const eNsGfxModel::ModelData& modelData,
			float tileSize
		);

		// TileMapActor(eNsECS::EntityMgr& ecs);
	};
}

