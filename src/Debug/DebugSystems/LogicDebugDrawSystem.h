// Debug drawing functions
#pragma once

#include "Core/ECS/EntityManager.h"
#include "Graphics/Renderer/RenderContext.h"

namespace LogicDebugDrawSystem
{
	void Draw(ECS& ecs, const RenderContext& renderContext);

	// Playerアクターの位置・マウスポインタの位置を描画(デバッグ用)
	void DebugDrawLogicPlayerPositions(ECS& ecs, const RenderContext& renderContext);

	// タイルマップのワイヤフレーム描画(デバッグ用)
	void DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext);

	// 2D円を描画(デバッグ用)
	void DebugDrawPlayerCollision(ECS& ecs, const RenderContext& renderContext);

	// TileMapとPlayerCircle2Dのコリジョンを描画
	void DebugDrawPlayerAndTileMap(ECS& ecs, const RenderContext& renderContext);

	// レンダーコンテキストをセット関数
	void SetOpenGLMatrixState(const RenderContext& renderContext);

	// レンダーコンテキストを解除する関数
	void ResetOpenGLMatrixState();
}