#include "GenericShape2DConverter.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include <variant>

Game::Collision::Data::GenericShape2D Game::Collision::Convert::MakeGenericShape2D(
	Engine::ECS::Entity e,
	Engine::ECS::EntityMgr& ecs)
{
	if (ecs.hasComponent<Engine::ECS::Component::Logic2D::CollisionComponent>(e)) {
		const auto& collider = ecs.get<Engine::ECS::Component::Logic2D::CollisionComponent>(e);
		if (ecs.hasComponent<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e)) {
			return MakeGenericShape2DFromTransform(collider.collider, ecs.get<Engine::ECS::Component::Logic2D::Logic2DTransformComponent>(e));
		}
	}
	else if (ecs.hasComponent<Game::Combat::Skill::Component::Attack2DAreaComponent>(e)) {
		const auto& area = ecs.get<Game::Combat::Skill::Component::Attack2DAreaComponent>(e);
		if (ecs.hasComponent<Engine::ECS::Component::Logic2D::Transform2DComponent>(e)) {
			return MakeGenericShape2DFromTransform(area.shape, ecs.get<Engine::ECS::Component::Logic2D::Transform2DComponent>(e));
		}
	}

	//この分岐に引っかかってないのが問題
	// 一旦解決（CollisionMaskを持っているが，Attack2DShapeもCollisionCompoentも持っていない(TileMapActor)Entityがある) => monostateを返す

	// assert(false && "No valid shape or transform");
	// std::cerr << "[MakeGenericShape2D] Warning: Entity " << e.id << " has no valid shape or transform.\n";
	return Game::Collision::Data::GenericShape2D{}; // monostate
}

Game::Collision::Data::GenericShape2D Game::Collision::Convert::BuildGenericShape2D(const Engine::ECS::Component::Logic2D::Collider& collider
	, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform)
{
	return std::visit([&](auto&& shape) -> Game::Collision::Data::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Circle2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			return Game::Collision::Data::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Obb2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 axisX = glm::normalize(shape.axisX);
			glm::vec2 axisZ = glm::normalize(shape.axisZ);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return Game::Collision::Data::FromObb(center, halfExtents, axisX, axisZ);
		}
		else if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Box2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return Game::Collision::Data::FromObb(center, halfExtents, transform.right, transform.front);
		}
		else
		{
			return Game::Collision::Data::GenericShape2D{};// monostate
		}
		}, collider.shape);
}

Game::Collision::Data::GenericShape2D Game::Collision::Convert::BuildGenericShape2D(const Game::Combat::Skill::Component::Attack2DShape& attackShape
	, const Engine::ECS::Component::Logic2D::Transform2DComponent& worldTransform)
{

	return std::visit([&](const auto& shape) -> Game::Collision::Data::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Circle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			return Game::Collision::Data::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Sector2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 direction = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			return Game::Collision::Data::FromSector(center, direction, shape.angle, shape.radius);
		}
		else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Rectangle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 front = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			glm::vec2 right = Engine::Math::Logic2D::CalcRightFromForward(front);
			glm::vec2 halfExtents = { shape.width * 0.5, shape.height * 0.5 };
			return Game::Collision::Data::FromObb(center, halfExtents, right, front);

		}
		else
		{
			return Game::Collision::Data::GenericShape2D{};// monostate
		}

		}, attackShape.shape);
}

// 未使用
Game::Collision::Data::GenericShape2D Game::Collision::Convert::ConvertFromCollider(const Engine::ECS::Component::Logic2D::Collider& collider
	, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform)
{
	return std::visit([&](auto&& shape) -> Game::Collision::Data::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Circle2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			return Game::Collision::Data::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Obb2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 axisX = glm::normalize(shape.axisX);
			glm::vec2 axisZ = glm::normalize(shape.axisZ);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return Game::Collision::Data::FromObb(center, halfExtents, axisX, axisZ);
		}
		else if constexpr (std::is_same_v<T, Engine::ECS::Component::Logic2D::Box2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return Game::Collision::Data::FromObb(center, halfExtents, transform.right, transform.front);
		}
		else
		{
			return Game::Collision::Data::GenericShape2D{};// monostate
		}
		}, collider.shape);
}

// 未使用
Game::Collision::Data::GenericShape2D Game::Collision::Convert::ConvertFromAttackShape(const Game::Combat::Skill::Component::Attack2DShape& attackShape
	, const Engine::ECS::Component::Logic2D::Transform2DComponent& worldTransform)
{
	return std::visit([&](const auto& shape) -> Game::Collision::Data::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Circle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			return Game::Collision::Data::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Sector2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 direction = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			return Game::Collision::Data::FromSector(center, direction, shape.angle, shape.radius);
		}
		else if constexpr (std::is_same_v<T, Game::Combat::Skill::Component::Rectangle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 front = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			glm::vec2 right = Engine::Math::Logic2D::CalcRightFromForward(front);
			glm::vec2 halfExtents = { shape.width * 0.5, shape.height * 0.5 };
			return Game::Collision::Data::FromObb(center, halfExtents, right, front);

		}
		else
		{
			return Game::Collision::Data::GenericShape2D{};// monostate
		}

		}, attackShape.shape);
}

glm::vec2 Game::Collision::Convert::applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform)
{
	//return transform.positionXZ
	//	+ transform.front * localOffset.y
	//	+ transform.right * localOffset.x;
	//return Engine::Math::Logic2D::Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.front, transform.right);
	// return transform.positionXZ + Engine::Math::Logic2D::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale
	//return transform.positionXZ
	//	+ Engine::Math::Logic2D::RotateVec2FromZForward(localOffset, transform.rotation) * transform.scale;
	
	return Engine::Math::Logic2D::Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.rotation, transform.scale);
}

glm::vec2 Game::Collision::Convert::applyLocalToWorldPoint(const glm::vec2& localOffset, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform)
{
	//return transform.positionXZ
	//	+ transform.front * localOffset.y
	//	+ transform.right * localOffset.x;// rightは初期化後更新していない(YAGNI: You Aren't Gonna Need it) 
	//return transform.positionXZ
	//	+ Engine::Math::Logic2D::RotateVec2FromZForward(localOffset, transform.rotationY) * transform.scale;

	//glm::vec2 front = Engine::Math::Logic2D::CalcForwardFromYaw(transform.rotationY);
	//glm::vec2 right = Engine::Math::Logic2D::CalcRightFromForward(front);
	//return transform.positionXZ
	//	+ front * localOffset.y
	//	+ right * localOffset.x;
	return Engine::Math::Logic2D::Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.rotationY, glm::vec2(transform.scale, transform.scale));
}

glm::vec2 Game::Collision::Convert::transformLocalPointToWorld(const glm::vec2& localDir, const Engine::ECS::Component::Logic2D::Logic2DTransformComponent& transform)
{
	return Engine::Math::Logic2D::Transform::TransformDirection(localDir, transform.rotation);;
}

glm::vec2 Game::Collision::Convert::transformLocalPointToWorld(const glm::vec2& localDir, const Engine::ECS::Component::Logic2D::Transform2DComponent& transform)
{
	return Engine::Math::Logic2D::Transform::TransformDirection(localDir, transform.rotationY); // rightは初期化後更新していない(YAGNI: You Aren't Gonna Need it) 
}