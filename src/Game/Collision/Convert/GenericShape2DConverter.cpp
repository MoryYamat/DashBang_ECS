#include "GenericShape2DConverter.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include <variant>

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

glm::vec2 Game::Collision::Convert::applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Logic2DTransformComponent& transfrom)
{
	return transfrom.positionXZ
		+ transfrom.front * localOffset.y
		+ transfrom.right * localOffset.x;
}

glm::vec2 Game::Collision::Convert::applyLocalToWorldPoint(const glm::vec2& localOffset, const eNsLogic2DComp::Transform2DComponent& transfrom)
{
	return transfrom.positionXZ
		+ transfrom.front * localOffset.y
		+ transfrom.right * localOffset.x;// rightは初期化後更新していない(YAGNI: You Aren't Gonna Need it) 
}

glm::vec2 Game::Collision::Convert::transformLocalPointToWorld(const glm::vec2& localDir, const eNsLogic2DComp::Logic2DTransformComponent& transfrom)
{
	return transfrom.front * localDir.y + transfrom.right * localDir.x;
}

glm::vec2 Game::Collision::Convert::transformLocalPointToWorld(const glm::vec2& localDir, const eNsLogic2DComp::Transform2DComponent& transfrom)
{
	return transfrom.front * localDir.y + transfrom.right * localDir.x;// rightは初期化後更新していない(YAGNI: You Aren't Gonna Need it) 
}