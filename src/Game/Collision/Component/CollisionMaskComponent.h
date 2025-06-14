// collision mask component

#pragma once 

#include "Game/Collision/Data/CollisionLayer.h"

#include <cstdint>

namespace Game::Collision::Component
{
	struct CollisionMaskComponent
	{
		Game::Collision::Data::Layer selfLayer = Game::Collision::Data::Layer::None;
		uint32_t collidesWithMask = 0; // 自分が当たりたい対象のLayerの集合 ORしたマスク (bit mask)
	};
}