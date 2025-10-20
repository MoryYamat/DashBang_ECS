#pragma once

#include <cstdint>

namespace Engine::World::Core
{
	struct Entity
	{
		uint32_t id = 0;
		
		constexpr Entity() = default;
		constexpr Entity(uint32_t id) : id(id){}

		bool operator== (const Entity& other) const noexcept
		{
			return id == other.id;
		}

		bool operator != (const Entity& other) const noexcept
		{
			return !(*this == other);
		}

		constexpr bool IsValid() noexcept { return id != 0; }

		// 
		static const Entity INVALID;
	};
}