// Debug drawing functions
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Graphics/Renderer/RenderContext.h"

#include "Game/CollisionLogic/Data/CollisionResultStorage.h"

namespace LogicDebugDrawSystem
{
	// デバッグ用描画のインターフェース
	void Draw(ECS& ecs,
		const RenderContext& renderContext,
		const CollisionResultStorage collisionResult);

	// Playerアクターの位置・マウスポインタの位置を描画(デバッグ用)
	void DebugDrawLogicPlayerPositions(ECS& ecs, const RenderContext& renderContext);

	// タイルマップのワイヤフレーム描画(デバッグ用)
	void DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext);

	// 2D円を描画(デバッグ用)
	void DebugDrawPlayerCollision(ECS& ecs, const RenderContext& renderContext);

	// TileMapとPlayerCircle2Dのコリジョンを描画
	void DebugDrawPlayerAndTileMap(ECS& ecs,
		const RenderContext& renderContext,
		const CollisionResultStorage collisionResult);

	// レンダーコンテキストをセット関数
	void SetOpenGLMatrixState(const RenderContext& renderContext);

	// レンダーコンテキストを解除する関数
	void ResetOpenGLMatrixState();
}