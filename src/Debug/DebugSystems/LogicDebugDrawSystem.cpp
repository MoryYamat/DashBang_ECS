#include "LogicDebugDrawSystem.h"

#include "Core/ECS/Component/Logic2DTransformComponent.h"
#include "Core/ECS/Component/TileMapComponent.h"

#include "Core/ECS/Component/Tags/PlayerControllerComponent.h"

#include "Core/ECS/Component/Transform2DComponent.h"

#include "Core/ECS/Component/Collision/CollisionComponent.h"

#include "Debug/DebugUtils.h"

// Game
#include "Game/CollisionLogic/TestCircleTileMapCollisionHighlight.h"
#include "Game/SkillSystem/Component/Attack2DAreaComponent.h"

#include "Game/SkillSystem/Utils/ShapeUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <variant>

// �f�o�b�O�p�i�_���n�j�̕`�悷��@�\�܂Ƃ߂Ă��
void LogicDebugDrawSystem::Draw(ECS& ecs, 
	const RenderContext& renderContext, 
	const CollisionResultStorage collisionResult
)
{

	SetOpenGLMatrixState(renderContext);

	//
	DebugDrawLogicTileMaps(ecs, renderContext);

	DebugDrawLogicPlayerPositions(ecs, renderContext);

	DebugDrawPlayerCollision(ecs, renderContext);
	
	// Debug�p�̃R���W�������m��\��
	LogicDebugDrawSystem::DebugDrawPlayerAndTileMap(ecs, renderContext, collisionResult);

	LogicDebugDrawSystem::SkillDaraw::RenderAttack2DAreas(ecs, renderContext);
	//
	//reset openGL matrix state
	ResetOpenGLMatrixState();
}

// �f�o�b�O�p�i�_�����W�j�̓_���`��`�悷��@�\���
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

// �f�o�b�O�p�A�^�C���}�b�v��`�悷��@�\���
void LogicDebugDrawSystem::DebugDrawLogicTileMaps(ECS& ecs, const RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	for (Entity e : ecs.view<TileMapComponent>())
	{
		const auto& tileMapComp = ecs.get<TileMapComponent>(e);

		for (int row = 0; row < tileMapComp.numRows; ++row)
		{
			for (int col = 0; col < tileMapComp.numCols; ++col)
			{
				const Tile& tile = tileMapComp.tiles[row][col];


				glm::vec2 center = tileMapComp.origin + glm::vec2(col + 0.5f, row + 0.5f) * tileMapComp.tileSize;

				glm::vec3 color;
				// walkable�����ɂ���ĕ`����@��ύX
				if (!tile.isWalkable)
				{
					color = glm::vec3(0.0f, 0.0f, 0.5f);
					DebugUtils::DebugDraw::DrawFilledQuad(center, tileMapComp.tileSize, color);
				}
				else
				{
					color = glm::vec3(1.0f, 1.0f, 0.0f);
					// �ǂ��ŉ��̏��𐶐����A�ǂ������`�œn������v����
					// �ǂ��ŉ��̏��𐶐����A�ǂ������`�œn������v����
					DebugUtils::DebugDraw::DrawQuad(center, tileMapComp.tileSize, color);
				}

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

void LogicDebugDrawSystem::DebugDrawPlayerAndTileMap(ECS& ecs, const RenderContext& renderContext, const CollisionResultStorage collisionResult)
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	TileMapComponent tileMapComp;
	for (Entity e : ecs.view<TileMapComponent>())
	{
		tileMapComp = ecs.get<TileMapComponent>(e);
		break;
	}

	//for (Entity e : ecs.view<CollisionComponent>())
	//{
	//	const auto& collisionComp = ecs.get<CollisionComponent>(e);

	//	// �����ł�����Ăяo���̂����������̂ł�����C������
	//	//CollisionUtils::TestCircleTileMapCollisionHighlight(collisionComp, tileMapComp);
	//}

	const auto& hitTileIndices = collisionResult.GetTileCollisions();


	for (const auto& idx : collisionResult.GetTileCollisions())
	{
		auto [tileMin, tileMax] = tileMapComp.GetTileAABB(idx.y, idx.x);

		// DebugUtils::LogVector_string("tilemin", tileMin);
		// DebugUtils::LogVector_string("tileMax", tileMax);

		DebugUtils::DebugDraw::DrawTileOutline(tileMin, tileMax, color);
	}

	// DebugUtils::DebugDraw::DrawTileOutline(collisionResult, tileMapComp, color);
}


void LogicDebugDrawSystem::SetOpenGLMatrixState(const RenderContext& renderContext)
{
	// view/projection �����O�ɃZ�b�g
	// �Œ�@�\�p�C�v���C���̌Â��@�\
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



void  LogicDebugDrawSystem::SkillDaraw::RenderAttack2DAreas(ECS& ecs, const RenderContext& renderContext)
{
	for (Entity e : ecs.view<Attack2DAreaComponent, Transform2DComponent>())
	{
		const auto& area = ecs.get<Attack2DAreaComponent>(e);
		const auto& transfrom = ecs.get<Transform2DComponent>(e);
		
		Attack2DShape worldShape = ShapeUtils::ComputeWorldShape(area.shape, transfrom);


		std::visit([&](const auto& shape)
			{
				using T = std::decay_t<decltype(shape)>;
				glm::vec4 color = glm::vec4(1.0f, 0.5f, 0.5f, 0.3f);


				if constexpr (std::is_same_v<T, Circle2DAttack>)
				{
					DebugUtils::DebugDraw::DrawFilledCircle2D(shape.center, shape.radius, color);
				}
				else if constexpr (std::is_same_v<T, Sector2DAttack>)
				{
					// DebugUtils::DebugDraw::DrawFilledSector2D(transfrom.positionXZ, transfrom.GetFrontXZ(), shape.radius, shape.angle, color);
					DebugUtils::DebugDraw::DrawFilledSector2D(shape.center, shape.direction, shape.radius, shape.angle, color);
				}
				else if constexpr (std::is_same_v<T, Rectangle2DAttack>)
				{
					DebugUtils::DebugDraw::DrawFilledRect2D(shape.center, shape.direction, shape.width, shape.height, color);
				}

			}, worldShape.shape);
	}
}