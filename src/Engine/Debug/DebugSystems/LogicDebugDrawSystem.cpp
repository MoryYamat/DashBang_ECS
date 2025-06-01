#include "LogicDebugDrawSystem.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Engine/Debug/DebugUtils.h"

// Game
#include "Game/Collision/TestCircleTileMapCollisionHighlight.h"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/Utils/ShapeUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <variant>

// デバッグ用（論理系）の描画する機能まとめてを提供
void Engine::Debug::Drawing::Logic2D::Draw(eNsECS::EntityMgr& ecs,
	const eNsGfxRender::RenderContext& renderContext,
	const Game::Collision::Data::CollisionResultStorage& collisionResult
)
{

	SetOpenGLMatrixState(renderContext);

	//
	DebugDrawLogicTileMaps(ecs, renderContext);

	DebugDrawLogicPlayerPositions(ecs, renderContext);

	DebugDrawPlayerCollision(ecs, renderContext);
	
	// Debug用のコリジョン検知を表示
	DebugDrawPlayerAndTileMap(ecs, renderContext, collisionResult);

	RenderAttack2DAreas(ecs, renderContext);
	//
	//reset openGL matrix state
	ResetOpenGLMatrixState();
}

// デバッグ用（論理座標）の点や矩形を描画する機能を提供
void Engine::Debug::Drawing::Logic2D::DebugDrawLogicPlayerPositions(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);


	for (eNsECS::Entity e : ecs.view<eNsLogic2DComp::Logic2DTransformComponent, eNsTagComp::PlayerControllerComponent>())
	{
		const auto& logic = ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e);

		glm::vec3 worldPos(logic.positionXZ.x, 0.01f, logic.positionXZ.y);

		eNsDebugDraw::DrawCross(worldPos, 0.1f, color);
	}

	//ResetOpenGLMatrixState();

}

// デバッグ用、タイルマップを描画する機能を提供
void Engine::Debug::Drawing::Logic2D::DebugDrawLogicTileMaps(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	for (eNsECS::Entity e : ecs.view<eNsLogic2DComp::TileMapComponent>())
	{
		const auto& tileMapComp = ecs.get<eNsLogic2DComp::TileMapComponent>(e);

		for (int row = 0; row < tileMapComp.numRows; ++row)
		{
			for (int col = 0; col < tileMapComp.numCols; ++col)
			{
				const eNsLogic2DComp::Tile& tile = tileMapComp.tiles[row][col];


				glm::vec2 center = tileMapComp.origin + glm::vec2(col + 0.5f, row + 0.5f) * tileMapComp.tileSize;

				glm::vec3 color;
				// walkable属性によって描画方法を変更
				if (!tile.isWalkable)
				{
					color = glm::vec3(0.0f, 0.0f, 0.5f);
					eNsDebugDraw::DrawFilledQuad(center, tileMapComp.tileSize, color);
				}
				else
				{
					color = glm::vec3(1.0f, 1.0f, 0.0f);
					// どこで何の情報を生成し、どういう形で渡すかを要検討
					// どこで何の情報を生成し、どういう形で渡すかを要検討
					eNsDebugDraw::DrawQuad(center, tileMapComp.tileSize, color);
				}

			}
		}
	}

	//ResetOpenGLMatrixState();

}

void Engine::Debug::Drawing::Logic2D::DebugDrawPlayerCollision(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext)
{
	glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f);

	for (eNsECS::Entity e : ecs.view<eNsLogic2DComp::CollisionComponent>())
	{
		const auto& collisionComp = ecs.get<eNsLogic2DComp::CollisionComponent>(e);


		if (collisionComp.collider.type == eNsLogic2DComp::ColliderType::Circle2D)
		{
			glm::vec2 center = collisionComp.collider.circle2D.center;
			float radius = collisionComp.collider.circle2D.radius;

			eNsDebugDraw::DrawCircle2D(center, radius, color);
		}
	}
}

void Engine::Debug::Drawing::Logic2D::DebugDrawPlayerAndTileMap(eNsECS::EntityMgr& ecs,
	const eNsGfxRender::RenderContext& renderContext,
	const Game::Collision::Data::CollisionResultStorage& collisionResult)
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	eNsLogic2DComp::TileMapComponent tileMapComp;
	for (eNsECS::Entity e : ecs.view<eNsLogic2DComp::TileMapComponent>())
	{
		tileMapComp = ecs.get<eNsLogic2DComp::TileMapComponent>(e);
		break;
	}

	//for (Entity e : ecs.view<CollisionComponent>())
	//{
	//	const auto& collisionComp = ecs.get<CollisionComponent>(e);

	//	// ここでこれを呼び出すのがおかしいのでそれを修正する
	//	//CollisionUtils::TestCircleTileMapCollisionHighlight(collisionComp, tileMapComp);
	//}

	const auto& hitTileIndices = collisionResult.GetTileCollisions();


	for (const auto& idx : collisionResult.GetTileCollisions())
	{
		auto [tileMin, tileMax] = tileMapComp.GetTileAABB(idx.y, idx.x);

		// DebugUtils::LogVector_string("tilemin", tileMin);
		// DebugUtils::LogVector_string("tileMax", tileMax);

		eNsDebugDraw::DrawTileOutline(tileMin, tileMax, color);
	}

	// DebugUtils::DebugDraw::DrawTileOutline(collisionResult, tileMapComp, color);
}


void Engine::Debug::Drawing::Logic2D::SetOpenGLMatrixState(const eNsGfxRender::RenderContext& renderContext)
{
	// view/projection を事前にセット
	// 固定機能パイプラインの古い機能
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(renderContext.projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(renderContext.viewMatrix));
}

void Engine::Debug::Drawing::Logic2D::ResetOpenGLMatrixState()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



void  Engine::Debug::Drawing::Logic2D::RenderAttack2DAreas(eNsECS::EntityMgr& ecs, const eNsGfxRender::RenderContext& renderContext)
{
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::Attack2DAreaComponent, eNsLogic2DComp::Transform2DComponent>())
	{
		const auto& area = ecs.get<gNsSkillComp::Attack2DAreaComponent>(e);
		const auto& transfrom = ecs.get<eNsLogic2DComp::Transform2DComponent>(e);
		
		gNsSkillComp::Attack2DShape worldShape = gNsSkillUtils::ComputeWorldShape(area.shape, transfrom);


		std::visit([&](const auto& shape)
			{
				using T = std::decay_t<decltype(shape)>;
				glm::vec4 color = glm::vec4(1.0f, 0.5f, 0.5f, 0.3f);


				if constexpr (std::is_same_v<T, gNsSkillComp::Circle2DAttack>)
				{
					eNsDebugDraw::DrawFilledCircle2D(shape.center, shape.radius, color);
				}
				else if constexpr (std::is_same_v<T, gNsSkillComp::Sector2DAttack>)
				{
					// DebugUtils::DebugDraw::DrawFilledSector2D(transfrom.positionXZ, transfrom.GetFrontXZ(), shape.radius, shape.angle, color);
					eNsDebugDraw::DrawFilledSector2D(shape.center, shape.direction, shape.radius, shape.angle, color);
				}
				else if constexpr (std::is_same_v<T, gNsSkillComp::Rectangle2DAttack>)
				{
					eNsDebugDraw::DrawFilledRect2D(shape.center, shape.direction, shape.width, shape.height, color);
				}

			}, worldShape.shape);
	}
}