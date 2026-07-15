#include "SDLInputDriver.h"

#include <SDL_assert.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <SDL_scancode.h>

namespace
{
    SDL_Scancode ToScancode(Key key)
    {
        switch (key)
        {
        case Key::A: return SDL_SCANCODE_A;
        case Key::B: return SDL_SCANCODE_B;
        case Key::C: return SDL_SCANCODE_C;
        case Key::D: return SDL_SCANCODE_D;
        case Key::E: return SDL_SCANCODE_E;
        case Key::F: return SDL_SCANCODE_F;
        case Key::G: return SDL_SCANCODE_G;
        case Key::H: return SDL_SCANCODE_H;
        case Key::I: return SDL_SCANCODE_I;
        case Key::J: return SDL_SCANCODE_J;
        case Key::K: return SDL_SCANCODE_K;
        case Key::L: return SDL_SCANCODE_L;
        case Key::M: return SDL_SCANCODE_M;
        case Key::N: return SDL_SCANCODE_N;
        case Key::O: return SDL_SCANCODE_O;
        case Key::P: return SDL_SCANCODE_P;
        case Key::Q: return SDL_SCANCODE_Q;
        case Key::R: return SDL_SCANCODE_R;
        case Key::S: return SDL_SCANCODE_S;
        case Key::T: return SDL_SCANCODE_T;
        case Key::U: return SDL_SCANCODE_U;
        case Key::V: return SDL_SCANCODE_V;
        case Key::W: return SDL_SCANCODE_W;
        case Key::X: return SDL_SCANCODE_X;
        case Key::Y: return SDL_SCANCODE_Y;
        case Key::Z: return SDL_SCANCODE_Z;

        case Key::Num0: return SDL_SCANCODE_0;
        case Key::Num1: return SDL_SCANCODE_1;
        case Key::Num2: return SDL_SCANCODE_2;
        case Key::Num3: return SDL_SCANCODE_3;
        case Key::Num4: return SDL_SCANCODE_4;
        case Key::Num5: return SDL_SCANCODE_5;
        case Key::Num6: return SDL_SCANCODE_6;
        case Key::Num7: return SDL_SCANCODE_7;
        case Key::Num8: return SDL_SCANCODE_8;
        case Key::Num9: return SDL_SCANCODE_9;

        case Key::Up: return SDL_SCANCODE_UP;
        case Key::Down: return SDL_SCANCODE_DOWN;
        case Key::Left: return SDL_SCANCODE_LEFT;
        case Key::Right: return SDL_SCANCODE_RIGHT;

        case Key::Escape: return SDL_SCANCODE_ESCAPE;
        case Key::Space: return SDL_SCANCODE_SPACE;
        case Key::Enter: return SDL_SCANCODE_RETURN;
        case Key::Backspace: return SDL_SCANCODE_BACKSPACE;
        case Key::Tab: return SDL_SCANCODE_TAB;

        case Key::LeftShift: return SDL_SCANCODE_LSHIFT;
        case Key::LeftCtrl: return SDL_SCANCODE_LCTRL;
        case Key::LeftAlt: return SDL_SCANCODE_LALT;
        case Key::RightShift: return SDL_SCANCODE_RSHIFT;
        case Key::RightCtrl: return SDL_SCANCODE_RCTRL;
        case Key::RightAlt: return SDL_SCANCODE_RALT;

        default: return SDL_SCANCODE_UNKNOWN;
        }
    }

    int ToSDLButton(MouseButton button)
    {
        switch (button)
        {
        case MouseButton::Left: return SDL_BUTTON_LEFT;
        case MouseButton::Middle: return SDL_BUTTON_MIDDLE;
        case MouseButton::Right: return SDL_BUTTON_RIGHT;
        case MouseButton::Forward: return SDL_BUTTON_X1;
        case MouseButton::Backward: return SDL_BUTTON_X2;

        default:
            SDL_assert(false && "ToSDLButton called with unmapped MouseButton");
            return 0;
        }
    }
}

std::array<bool, static_cast<size_t>(Key::Count)> SDLInputDriver::GetKeyStates() const
{
    std::array<bool, static_cast<size_t>(Key::Count)> states{};
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

    for (size_t i = 1; i < states.size(); ++i)
    {
        SDL_Scancode scancode = ToScancode(static_cast<Key>(i));
        states[i] = keyboardState[scancode] != 0;
    }

    return states;
}

std::array<bool, static_cast<size_t>(MouseButton::Count)> SDLInputDriver::GetMouseButtonStates() const
{
    std::array<bool, static_cast<size_t>(MouseButton::Count)> states{};
    int x, y;
    Uint32 buttonState = SDL_GetMouseState(&x, &y);

    for (size_t i = 1; i < states.size(); ++i)
    {
        int sdlButton = ToSDLButton(static_cast<MouseButton>(i));
        states[i] = (buttonState & SDL_BUTTON(sdlButton)) != 0;
    }

    return states;
}

glm::vec2 SDLInputDriver::GetMousePosition() const
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return { static_cast<float>(x), static_cast<float>(y) };
}
