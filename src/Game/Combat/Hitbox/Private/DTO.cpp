#include "Game/Combat/HitBox/Public/DTO.hpp"
#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"

#include "Game/Combat/HitBox/Private/HitBoxBuilder.hpp"

namespace {

	using namespace Game::Combat::HitBox;

	void make_HitBoxDTO(HitBoxBuilder& builder)
	{
		HitBoxDTO default_obb2d;
		default_obb2d.name = "default_obb2D";
		default_obb2d.shape = "Obb2D";
		default_obb2d.offsetX = 0.f;
		default_obb2d.offsetY = 0.f;
		default_obb2d.vert = 6.f;
		default_obb2d.horizon = 2.f;

		builder.add(default_obb2d);

		// 以下 随時形状追加
	}
}

namespace Game::Combat::HitBox
{
	void RegisterHitBoxDTO(HitBoxBuilder& builder)
	{
		make_HitBoxDTO(builder);
	}
}