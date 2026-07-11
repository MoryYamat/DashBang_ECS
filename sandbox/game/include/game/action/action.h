#pragma once

#include <cstdint>
#include <cstddef>

namespace app::action
{

    // good usecase
    enum class Action : std::uint32_t
    {
        // move
        MoveForward = 0,
        MoveBackward,
        MoveLeft,
        MoveRight,

        CameraLookX,
        CameraLookY,

        CameraZoom,

        // action
        Attack,
    };

    // struct ActionType
    // {
    //     std::size_t v;

    //     explicit ActionType(std::size_t v) : v(v) {}

    //     explicit operator std::size_t() const
    //     {
    //         return v;
    //     }
    // };
    // inline ActionType move_forward{0};

} // namespace app::action