// Debug drawing functions
#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Game/Collision/Data/CollisionResultStorage.h"

#include "Common/EngineNamespaceDecl.h"
#include "Common/GameNamespaceDecl.h"

namespace Engine::Debug::Drawing::Logic2D
{
	// デバッグ用描画のインターフェース
	void Draw(eNsECS::EntityMgr& ecs,
		const eNsGfxRender::RenderContext& renderContext,
		const Game::Collision::Data::CollisionResultStorage& collisionResult);

	// Playerアクターの位置・マウスポインタの位置を描画(デバッグ用)
	void DebugDrawLogicPlayerPositions(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext);

	// タイルマップのワイヤフレーム描画(デバッグ用)
	void DebugDrawLogicTileMaps(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext);

	// 2D円を描画(デバッグ用)
	void DebugDrawPlayerCollision(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext);


	// TileMapとPlayerCircle2Dのコリジョンを描画
	void DebugDrawPlayerAndTileMap(eNsECS::EntityMgr& ecs,
		const eNsGfxRender::RenderContext& renderContext,
		const Game::Collision::Data::CollisionResultStorage& collisionResult);

	// レンダーコンテキストをセット関数
	void SetOpenGLMatrixState(const eNsGfxRender::RenderContext& renderContext);

	// レンダーコンテキストを解除する関数
	void ResetOpenGLMatrixState();

}


// ファイルを「システム用デバッグ描画」と「ゲーム用デバッグ描画」に分ける
// ファイルを「システム用デバッグ描画」と「ゲーム用デバッグ描画」に分ける
namespace Engine::Debug::Drawing::Logic2D
{
	// Skillの論理的効果範囲形状を描画する
	void RenderAttack2DAreas(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext);
}