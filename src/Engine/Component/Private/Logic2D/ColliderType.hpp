#pragma once

#include <cstdint>

namespace Engine::Component
{
	enum class ShapeKind : std::uint8_t
	{
		None,
		Circle2D,
		Box2D,
		Obb2D,
		// Sector2D, ... 
	};
}