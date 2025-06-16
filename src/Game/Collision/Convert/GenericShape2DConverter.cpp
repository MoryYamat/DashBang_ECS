#include "GenericShape2DConverter.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include <variant>

gNsCollData::GenericShape2D Game::Collision::Convert::MakeGenericShape2D(
	eNsECS::Entity e,
	eNsECS::EntityMgr& ecs)
{
	if (ecs.hasComponent<eNsLogic2DComp::CollisionComponent>(e)) {
		const auto& collider = ecs.get<eNsLogic2DComp::CollisionComponent>(e);
		if (ecs.hasComponent<eNsLogic2DComp::Logic2DTransformComponent>(e)) {
			return MakeGenericShape2DFromTransform(collider.collider, ecs.get<eNsLogic2DComp::Logic2DTransformComponent>(e));
		}
	}
	else if (ecs.hasComponent<gNsSkillComp::Attack2DAreaComponent>(e)) {
		const auto& area = ecs.get<gNsSkillComp::Attack2DAreaComponent>(e);
		if (ecs.hasComponent<eNsLogic2DComp::Transform2DComponent>(e)) {
			return MakeGenericShape2DFromTransform(area.shape, ecs.get<eNsLogic2DComp::Transform2DComponent>(e));
		}
	}

	//この分岐に引っかかってないのが問題
	// 一旦解決（CollisionMaskを持っているが，Attack2DShapeもCollisionCompoentも持っていない(TileMapActor)Entityがある) => monostateを返す

	// assert(false && "No valid shape or transform");
	// std::cerr << "[MakeGenericShape2D] Warning: Entity " << e.id << " has no valid shape or transform.\n";
	return gNsCollData::GenericShape2D{}; // monostate
}

gNsCollData::GenericShape2D Game::Collision::Convert::BuildGenericShape2D(const eNsLogic2DComp::Collider& collider
	, const eNsLogic2DComp::Logic2DTransformComponent& transform)
{
	return std::visit([&](auto&& shape) -> gNsCollData::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, eNsLogic2DComp::Circle2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			return gNsCollData::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, eNsLogic2DComp::Obb2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 axisX = glm::normalize(shape.axisX);
			glm::vec2 axisZ = glm::normalize(shape.axisZ);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return gNsCollData::FromObb(center, halfExtents, axisX, axisZ);
		}
		else if constexpr (std::is_same_v<T, eNsLogic2DComp::Box2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return gNsCollData::FromObb(center, halfExtents, transform.right, transform.front);
		}
		else
		{
			return gNsCollData::GenericShape2D{};// monostate
		}
		}, collider.shape);
}

gNsCollData::GenericShape2D Game::Collision::Convert::BuildGenericShape2D(const gNsSkillComp::Attack2DShape& attackShape
	, const eNsLogic2DComp::Transform2DComponent& worldTransform)
{

	return std::visit([&](const auto& shape) -> gNsCollData::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, gNsSkillComp::Circle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			return gNsCollData::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, gNsSkillComp::Sector2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 direction = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			return gNsCollData::FromSector(center, direction, shape.angle, shape.radius);
		}
		else if constexpr (std::is_same_v<T, gNsSkillComp::Rectangle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 front = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			glm::vec2 right = eNsLogic2DMath::CalcRightFromForward(front);
			glm::vec2 halfExtents = { shape.width * 0.5, shape.height * 0.5 };
			return gNsCollData::FromObb(center, halfExtents, right, front);

		}
		else
		{
			return gNsCollData::GenericShape2D{};// monostate
		}

		}, attackShape.shape);
}

// 未使用
gNsCollData::GenericShape2D Game::Collision::Convert::ConvertFromCollider(const eNsLogic2DComp::Collider& collider
	, const eNsLogic2DComp::Logic2DTransformComponent& transform)
{
	return std::visit([&](auto&& shape) -> gNsCollData::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, eNsLogic2DComp::Circle2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			return gNsCollData::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, eNsLogic2DComp::Obb2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 axisX = glm::normalize(shape.axisX);
			glm::vec2 axisZ = glm::normalize(shape.axisZ);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return gNsCollData::FromObb(center, halfExtents, axisX, axisZ);
		}
		else if constexpr (std::is_same_v<T, eNsLogic2DComp::Box2D>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, transform);
			glm::vec2 halfExtents = shape.halfExtents * transform.scale;
			return gNsCollData::FromObb(center, halfExtents, transform.right, transform.front);
		}
		else
		{
			return gNsCollData::GenericShape2D{};// monostate
		}
		}, collider.shape);
}

// 未使用
gNsCollData::GenericShape2D Game::Collision::Convert::ConvertFromAttackShape(const gNsSkillComp::Attack2DShape& attackShape
	, const eNsLogic2DComp::Transform2DComponent& worldTransform)
{
	return std::visit([&](const auto& shape) -> gNsCollData::GenericShape2D {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, gNsSkillComp::Circle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			return gNsCollData::FromCircle(center, shape.radius);
		}
		else if constexpr (std::is_same_v<T, gNsSkillComp::Sector2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 direction = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			return gNsCollData::FromSector(center, direction, shape.angle, shape.radius);
		}
		else if constexpr (std::is_same_v<T, gNsSkillComp::Rectangle2DAttack>)
		{
			// 2Dローカルオフセットをワールド座標に変換（XZ平面）
			glm::vec2 center = applyLocalToWorldPoint(shape.center, worldTransform);
			glm::vec2 front = glm::normalize(transformLocalPointToWorld(shape.direction, worldTransform));
			glm::vec2 right = eNsLogic2DMath::CalcRightFromForward(front);
			glm::vec2 halfExtents = { shape.width * 0.5, shape.height * 0.5 };
			return gNsCollData::FromObb(center, halfExtents, right, front);

		}
		else
		{
			return gNsCollData::GenericShape2D{};// monostate
		}

		}, attackShape.shape);
}

glm::vec2 Game::Collision::Convert::applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Logic2DTransformComponent& transform)
{
	//return transform.positionXZ
	//	+ transform.front * localOffset.y
	//	+ transform.right * localOffset.x;
	//return eNsLogic2DMath::Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.front, transform.right);
	// return transform.positionXZ + eNsLogic2DMath::RotateVec2FromZForward(result.center, transform.rotationY) * transform.scale
	//return transform.positionXZ
	//	+ eNsLogic2DMath::RotateVec2FromZForward(localOffset, transform.rotation) * transform.scale;
	
	return eNsLogic2DMath::Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.rotation, transform.scale);
}

glm::vec2 Game::Collision::Convert::applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Transform2DComponent& transform)
{
	//return transform.positionXZ
	//	+ transform.front * localOffset.y
	//	+ transform.right * localOffset.x;// rightは初期化後更新していない(YAGNI: You Aren't Gonna Need it) 
	//return transform.positionXZ
	//	+ eNsLogic2DMath::RotateVec2FromZForward(localOffset, transform.rotationY) * transform.scale;

	//glm::vec2 front = eNsLogic2DMath::CalcForwardFromYaw(transform.rotationY);
	//glm::vec2 right = eNsLogic2DMath::CalcRightFromForward(front);
	//return transform.positionXZ
	//	+ front * localOffset.y
	//	+ right * localOffset.x;
	return eNsLogic2DMath::Transform::ApplyLocalOffset(localOffset, transform.positionXZ, transform.rotationY, glm::vec2(transform.scale, transform.scale));
}

glm::vec2 Game::Collision::Convert::transformLocalPointToWorld(const glm::vec2& localDir, const eNsLogic2DComp::Logic2DTransformComponent& transform)
{
	return eNsLogic2DMath::Transform::TransformDirection(localDir, transform.rotation);;
}

glm::vec2 Game::Collision::Convert::transformLocalPointToWorld(const glm::vec2& localDir, const eNsLogic2DComp::Transform2DComponent& transform)
{
	return eNsLogic2DMath::Transform::TransformDirection(localDir, transform.rotationY); // rightは初期化後更新していない(YAGNI: You Aren't Gonna Need it) 
}