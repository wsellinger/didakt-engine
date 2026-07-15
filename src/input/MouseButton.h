#pragma once

#include <cstdint>

enum class MouseButton : uint8_t
{
    Unknown = 0,

    Left, Middle, Right,
    Forward, Backward,

    Count
};