// collision mask component

#pragma once 

#include "Game/Collision/Data/CollisionLayer.h"

#include <cstdint>

namespace Game::Collision::Component
{
	struct CollisionMaskComponent
	{
		Game::Collision::Data::Layer selfLayer = Game::Collision::Data::Layer::None;
		uint8_t collidesWithMask = 0; // ORÇµÇΩÉ}ÉXÉN (bit mask)
	};
}