#include "LogicDebugDrawSystem.h"

#include "Engine/ECS/Component/Common/TransformComponent.h"

#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"
#include "Engine/ECS/Component/Logic2D/TileMapComponent.h"

#include "Engine/ECS/Component/Tags/PlayerControllerComponent.h"


#include "Engine/ECS/Component/Logic2D/Transform2DComponent.h"

#include "Engine/ECS/Component/Logic2D/CollisionComponent.h"

#include "Engine/ECS/Component/Input/AnalogInputComponent.h"

#include "Engine/Debug/Private/DebugUtils.h"

#include "Engine/Graphics/Private/Renderer/RenderContext.h"

// Game
#include "Game/Collision/TestCircleTileMapCollisionHighlight.h"
#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Game/Combat/Skill/Utils/ShapeUtils.h"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <variant>

// デバッグ用（論理系）の描画する機能まとめてを提供
void Engine::Debug::Drawing::Logic2D::Draw(Engine::ECS::EntityMgr& ecs,
	const Engine::Graphics::Render::RenderContext& renderContext,
	const Game::Collision::Data::CollisionResultStorage& collisionResult
)
{

	SetOpenGLMatrixState(renderContext);

	//
	DebugDrawLogicTileMaps(ecs, renderContext);

	// PlayerCharacterとマウスカーソルのXZ平面上の位置を十字で描画
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
void Engine::Debug::Drawing::Logic2D::DebugDrawLogicPlayerPositions(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);


	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Logic2D::Logic2DTransformComponent, Game::ECS::Tags::PlayerCharacterTag>())
	{
		const auto& logic = ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e);

		glm::vec3 worldPos(logic.positionXZ.x, 0.01f, logic.positionXZ.y);

		Engine::Debug::Drawing::DrawCross(worldPos, 0.1f, color);
	}

	// mouse cursor position
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Input::AnalogInputComponent>())
	{
		const auto& analogInput = ecs.get<Engine::ECS::Component::Input::AnalogInputComponent>(e);

		glm::vec3 worldPos(analogInput.cursorLogicPositionXZ.x, 0.01f, analogInput.cursorLogicPositionXZ.y);

		Engine::Debug::Drawing::DrawCross(worldPos, 0.1f, color);
	}

	//ResetOpenGLMatrixState();

}

// デバッグ用、タイルマップを描画する機能を提供
void Engine::Debug::Drawing::Logic2D::DebugDrawLogicTileMaps(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext)
{
	//SetOpenGLMatrixState(renderContext);


	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Logic2D::TileMapComponent>())
	{
		const auto& tileMapComp = ecs.get<Engine::ECS::Component::Logic2D::TileMapComponent>(e);

		for (int row = 0; row < tileMapComp.numRows; ++row)
		{
			for (int col = 0; col < tileMapComp.numCols; ++col)
			{
				const Engine::ECS::Component::Logic2D::Tile& tile = tileMapComp.tiles[row][col];


				glm::vec2 center = tileMapComp.origin + glm::vec2(col + 0.5f, row + 0.5f) * tileMapComp.tileSize;

				glm::vec3 color;
				// walkable属性によって描画方法を変更
				if (!tile.isWalkable)
				{
					color = glm::vec3(0.0f, 0.0f, 0.5f);
					Engine::Debug::Drawing::DrawFilledQuad(center, tileMapComp.tileSize, color);
				}
				else
				{
					color = glm::vec3(1.0f, 1.0f, 0.0f);
					// どこで何の情報を生成し、どういう形で渡すかを要検討
					// どこで何の情報を生成し、どういう形で渡すかを要検討
					Engine::Debug::Drawing::DrawQuad(center, tileMapComp.tileSize, color);
				}

			}
		}
	}

	//ResetOpenGLMatrixState();

}

// uion使用時の条件分岐
//void Engine::Debug::Drawing::Logic2D::DebugDrawPlayerCollision(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext)
//{
//	glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f);
//
//	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Logic2D::CollisionComponent>())
//	{
//		const auto& collisionComp = ecs.get<Engine::ECS::Component::Logic2D::CollisionComponent>(e);
//
//
//		if (collisionComp.collider.type == Engine::ECS::Component::Logic2D::ColliderType::Circle2D)
//		{
//			glm::vec2 center = collisionComp.collider.circle2D.center;
//			float radius = collisionComp.collider.circle2D.radius;
//
//			Engine::Debug::Drawing::DrawCircle2D(center, radius, color);
//		}
//	}
//}

// variantの条件分岐
void Engine::Debug::Drawing::Logic2D::DebugDrawPlayerCollision(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext)
{
	glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f);

	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Logic2D::CollisionComponent, Engine::ECS::Component::Common::TransformComponent>())
	{
		const auto& collisionComp = ecs.get<Engine::ECS::Component::Logic2D::CollisionComponent>(e);
		const auto& transformComp = ecs.get<Engine::ECS::Component::Common::TransformComponent>(e);

		std::visit([&](auto&& shape) {
			using T = std::decay_t<decltype(shape)>;
			if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Circle2D>)
			{
				// ローカル -> ワールド変換
				glm::vec2 worldCenter = glm::vec2(transformComp.position.x, transformComp.position.z)
					+ shape.center;// ローカルオフセット

				float worldRadius = shape.radius;// 必要ならスケーリング(ローカルにスケーリングを適用済(修正必要))

				Engine::Debug::Drawing::DrawCircle2D(worldCenter, worldRadius, color);
			}
			}, collisionComp.collider.shape);
	}
}

void Engine::Debug::Drawing::Logic2D::DebugDrawPlayerAndTileMap(Engine::ECS::EntityMgr& ecs,
	const Engine::Graphics::Render::RenderContext& renderContext,
	const Game::Collision::Data::CollisionResultStorage& collisionResult)
{
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

	Engine::ECS::Component::Logic2D::TileMapComponent tileMapComp;
	for (Engine::ECS::Entity e : ecs.view<Engine::ECS::Component::Logic2D::TileMapComponent>())
	{
		tileMapComp = ecs.get<Engine::ECS::Component::Logic2D::TileMapComponent>(e);
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

		Engine::Debug::Drawing::DrawTileOutline(tileMin, tileMax, color);
	}

	// DebugUtils::DebugDraw::DrawTileOutline(collisionResult, tileMapComp, color);
}


void Engine::Debug::Drawing::Logic2D::SetOpenGLMatrixState(const Engine::Graphics::Render::RenderContext& renderContext)
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



void  Engine::Debug::Drawing::Logic2D::RenderAttack2DAreas(Engine::ECS::EntityMgr& ecs, const Engine::Graphics::Render::RenderContext& renderContext)
{
	for (Engine::ECS::Entity e : ecs.view<Game::Combat::Skill::Component::Attack2DAreaComponent, Engine::ECS::Component::Logic2D::Transform2DComponent>())
	{
		const auto& area = ecs.get<Game::Combat::Skill::Component::Attack2DAreaComponent>(e);
		const auto& transfrom = ecs.get<Engine::ECS::Component::Logic2D::Transform2DComponent>(e);
		
		Game::Combat::Skill::Component::Attack2DShape worldShape = Game::Combat::Skill::Utils::ComputeWorldShape(area.shape, transfrom);


		std::visit([&](const auto& shape)
			{
				using T = std::decay_t<decltype(shape)>;
				glm::vec4 color = glm::vec4(1.0f, 0.5f, 0.5f, 0.3f);


				if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Circle2DAttack>)
				{
					Engine::Debug::Drawing::DrawFilledCircle2D(shape.center, shape.radius, color);
				}
				else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Sector2DAttack>)
				{
					// DebugUtils::DebugDraw::DrawFilledSector2D(transfrom.positionXZ, transfrom.GetFrontXZ(), shape.radius, shape.angle, color);
					Engine::Debug::Drawing::DrawFilledSector2D(shape.center, shape.direction, shape.radius, shape.angle, color);
				}
				else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Rectangle2DAttack>)
				{
					Engine::Debug::Drawing::DrawFilledRect2D(shape.center, shape.direction, shape.width, shape.height, color);
				}

			}, worldShape.shape);
	}
}