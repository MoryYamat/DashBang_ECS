#include "Game/Combat/HitBox/Private/HitBoxBuilder.hpp"
#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"


#include "Engine/Component/Private/Logic2D/ColliderType.hpp"

#include "Engine/Log/Public/LogApi.hpp"
#include "Engine/Math/Private/MathUtils.h"

#include <string_view>
#include <cstddef>

namespace
{
	using namespace Engine::Component;
	bool ParseShapeKind(std::string_view s, ShapeKind& out)
	{
		if (s == "Circle2D")
		{
			out = ShapeKind::Circle2D;
			return true;
		}
		if (s == "Box2D")
		{
			out = ShapeKind::Box2D;
			return true;
		}
		if (s == "Obb2D")
		{
			out = ShapeKind::Obb2D;
			return true;
		}
		if (s == "None")
		{
			out = ShapeKind::None;
			return true;
		}

		return false;
	}
}

namespace Game::Combat::HitBox
{
	using namespace Game::Combat::Skill;
	HitBoxBuildResult HitBoxBuilder::build()
	{
		HitBoxBuildResult result;

		result.catalog.hitBoxes.resize(defs_.size());// ?

		for (std::size_t i = 0; i< defs_.size(); ++i)
		{
			const auto& dto = defs_[i];

			HitBoxID id{};
			id.v = static_cast<std::uint16_t>(i);

			CanonicalHitBoxData& dst = result.catalog.hitBoxes[i];
			if (!buildOne(dto, id, dst, result.errs))
			{
				// 
			}

			// name -> id マップ
			auto [it, inserted] =
				result.catalog.nameToId.emplace(dto.name, id);
			if (!inserted)
			{
				result.errs.err("HitBox name '" + dto.name
					+ "' is duplicated");
			}
		}

		return result;
	}


	bool HitBoxBuilder::buildOne(
		const HitBoxDTO& dto,
		HitBoxID id,
		CanonicalHitBoxData& out,
		HitBoxBuilderErrors& err
	)
	{
		bool ok = true;
		out.id = id;
		
		// shape 判定
		ShapeKind shape;
		if (!ParseShapeKind(dto.shape, shape))
		{
			err.err("HitBox '" + dto.name +
				"': unknown shape '" + dto.shape + "'");
			
			return false;
		}
		out.shape = shape;
		out.offset = glm::vec2(dto.offsetX, dto.offsetY);

		// 形状ごとに妥当性チェック + 値コピー
		switch (shape)
		{
		case ShapeKind::None:
			break;
		case ShapeKind::Circle2D:
			if (dto.radius <= 0.f)
			{
				err.err("HitBox '" + dto.name +
					"': Circle requires radius > 0");
				ok = false;
			}
			out.radius = dto.radius;
			out.angle = Engine::Math::DegreesToRadians(0.f);
			out.length = 0.f;
			break;
		case ShapeKind::Box2D:
			if (dto.length <= 0.f || dto.radius <= 0.f)
			{
				err.err("HitBox '" + dto.name +
					"': Box2D requires length > 0 and radius(half-width) > 0");
				ok = false;
			}
			out.length = dto.length;
			out.radius = dto.radius;
			out.angle = Engine::Math::DegreesToRadians(0.f);
			break;
		case ShapeKind::Obb2D:
			if (dto.vert <= 0.f || dto.horizon <= 0.f)
			{
				err.err("HitBox '" + dto.name +
					"': Obb2D requires vert > 0 and horizon > 0");
				ok = false;
			}
			out.vert = dto.vert;
			out.horilzon = dto.horizon;
			out.angle = Engine::Math::DegreesToRadians(dto.angle);
			break;
		default:
			break;
		}

		return ok;
	}
}