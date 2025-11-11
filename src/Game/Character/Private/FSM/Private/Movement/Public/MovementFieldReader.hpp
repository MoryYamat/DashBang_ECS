#pragma once

#include "Engine/FSM/Public/FSMApi.hpp"

#include  "MovementFields.hpp"

#include <cstdint>

namespace Game::Character::FSM::Movement
{
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