#pragma once

#include "Engine/FSM/Public/FSMApi.hpp"


#include <cstdint>

namespace Game::Character::FSM::Movement
{
	enum class Field : std::uint16_t
	{
		MovementInputMag = 0,
		Count
	};

	constexpr std::uint16_t to_index(Field f)
	{
		return static_cast<std::uint16_t>(f);
	}

	// 逆引きやデバッグ用
	constexpr std::string_view to_name(Field f)
	{
		switch (f)
		{
		case Field::MovementInputMag: return "movementInputMag";
		default: return "";
		}
	}

	struct MovementFieldReader : Engine::FSM::Core::IFieldReader {
		float movementInputMag = 0.f;
		float getF32(std::uint16_t fieldIndex) const override {
			switch (static_cast<Field>(fieldIndex)) {
			case Field::MovementInputMag: return movementInputMag;
			default: return 0.f;
			}
		}
	};
}