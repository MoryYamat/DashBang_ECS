#pragma once

#include <cstdint>

namespace app::action
{

    enum class Action : std::uint32_t
    {
        // move
        MoveFoward=0,
        MoveBackward,
        MoveLeft,
        MoveRight
    };



}// namespace app::action