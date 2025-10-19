// Debug drawing functions
#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Game/Collision/Data/CollisionResultStorage.h"


namespace Engine::Debug::Drawing::Logic2D
{
	// デバッグ用描画のインターフェース
	void Draw(Engine::ECS::EntityMgr& ecs,
		const Engine::Graphics::Render::RenderContext& renderContext,
		const Game::Collision::Data::CollisionResultStorage& collisionResult);

	// Playerアクターの位置・マウスポインタの位置を描画(デバッグ用)
	void DebugDrawLogicPlayerPositions(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext);

	// タイルマップのワイヤフレーム描画(デバッグ用)
	void DebugDrawLogicTileMaps(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext);

	// 2D円を描画(デバッグ用)
	void DebugDrawPlayerCollision(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext);


	// TileMapとPlayerCircle2Dのコリジョンを描画
	void DebugDrawPlayerAndTileMap(Engine::ECS::EntityMgr& ecs,
		const Engine::Graphics::Render::RenderContext& renderContext,
		const Game::Collision::Data::CollisionResultStorage& collisionResult);

	// レンダーコンテキストをセット関数
	void SetOpenGLMatrixState(const Engine::Graphics::Render::RenderContext& renderContext);

	// レンダーコンテキストを解除する関数
	void ResetOpenGLMatrixState();

}


// ファイルを「システム用デバッグ描画」と「ゲーム用デバッグ描画」に分ける
// ファイルを「システム用デバッグ描画」と「ゲーム用デバッグ描画」に分ける
namespace Engine::Debug::Drawing::Logic2D
{
	// Skillの論理的効果範囲形状を描画する
	void RenderAttack2DAreas(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext);
}