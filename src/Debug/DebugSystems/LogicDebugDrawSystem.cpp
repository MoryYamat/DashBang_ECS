#include "LogicDebugDrawSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/TileMapComponent.h"

#include "Core/ECS/Component/PlayerControllerComponent.h"

#include "Core/ECS/Component/Collision/CollisionComponent.h"

#include "Debug/DebugUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// デバッグ用（論理系）の描画する機能まとめてを提供
void LogicDebugDrawSystem::Draw(ECS& ecs, const RenderContext& renderContext)
{

	SetOpenGLMatrixState(renderContext);

	//
	DebugDrawLogicTileMaps(ecs, renderContext);

	DebugDrawLogicPlayerPositions(ecs, renderContext);

	DebugDrawPlayerCollision(ecs, renderContext);

	//
	//reset openGL matrix state
	ResetOpenGLMatrixState();
}

// デバッグ用（論理座標）の点や矩形を描画する機能を提供
void LogicDebugDrawSystem::DebugDrawLogicPlayerPositions(ECS& ecs, const RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);


	for (Entity e : ecs.view<Logic2DTransformComponent, PlayerControllerComponent>())
	{
		const auto& logic = ecs.get<Logic2DTransformComponent>(e);

		glm::vec3 worldPos(logic.positionXZ.x, 0.01f, logic.positionXZ.y);

		DebugUtils::DebugDraw::DrawCross(worldPos, 0.1f, color);
	}

	//ResetOpenGLMatrixState();

}

// デバッグ用、タイルマップを描画する機能を提供
void LogicDebugDrawSystem::DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

	for (Entity e : ecs.view<TileMapComponent>())
	{
		const auto& tileMapComp = ecs.get<TileMapComponent>(e);

		for (int row = 0; row < tileMapComp.numRows; ++row)
		{
			for (int col = 0; col < tileMapComp.numCols; ++col)
			{
				// どこで何の情報を生成し、どういう形で渡すかを要検討
				// どこで何の情報を生成し、どういう形で渡すかを要検討
				glm::vec2 center = tileMapComp.origin + glm::vec2(col + 0.5f, row + 0.5f) * tileMapComp.tileSize;

				DebugUtils::DebugDraw::DrawQuad(center, tileMapComp.tileSize, color);
			}
		}
	}

	//ResetOpenGLMatrixState();

}

void LogicDebugDrawSystem::DebugDrawPlayerCollision(ECS& ecs, const RenderContext& renderContext)
{
	glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f);

	for (Entity e : ecs.view<CollisionComponent>())
	{
		const auto& collisionComp = ecs.get<CollisionComponent>(e);

		if (collisionComp.collider.type == ColliderType::Circle2D)
		{
			glm::vec2 center = collisionComp.collider.circle2D.center;
			float radius = collisionComp.collider.circle2D.radius;

			DebugUtils::DebugDraw::DrawCircle2D(center, radius, color);
		}
	}
}

void LogicDebugDrawSystem::SetOpenGLMatrixState(const RenderContext& renderContext)
{
	// view/projection を事前にセット
	// 固定機能パイプラインの古い機能
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(renderContext.projectionMatrix));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(renderContext.viewMatrix));
}

void LogicDebugDrawSystem::ResetOpenGLMatrixState()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}