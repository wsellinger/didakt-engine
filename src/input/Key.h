#pragma once

#include <cstdint>

enum class Key : uint8_t
{
    Unknown = 0,

    //Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    //Digits
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

    //Arrow Keys
    Up, Down, Left, Right,

    //Control Keys
    Escape, Space, Enter, Backspace, Tab, 
    LeftShift, LeftCtrl, LeftAlt,
    RightShift, RightCtrl, RightAlt,

    Count
};