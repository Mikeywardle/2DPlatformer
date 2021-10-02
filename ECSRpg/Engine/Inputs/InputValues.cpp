#include "InputValues.h"

#include <GLFW/glfw3.h>


//InputTypes
const InputType InputTypes::BUTTON_IS_DOWN = 0;
const InputType InputTypes::BUTTON_RELEASED = 1;
const InputType InputTypes::BUTTON_PRESSED = 2;
const InputType InputTypes::MAX_NUMBER_OF_TYPES = 3; //Always largest type +1

//Letter Keys
const InputKey InputKeys::KEYBOARD_A_KEY = GLFW_KEY_A;
const InputKey InputKeys::KEYBOARD_B_KEY = GLFW_KEY_B;
const InputKey InputKeys::KEYBOARD_C_KEY = GLFW_KEY_C;
const InputKey InputKeys::KEYBOARD_D_KEY = GLFW_KEY_D;
const InputKey InputKeys::KEYBOARD_E_KEY = GLFW_KEY_E;
const InputKey InputKeys::KEYBOARD_F_KEY = GLFW_KEY_F;
const InputKey InputKeys::KEYBOARD_G_KEY = GLFW_KEY_G;
const InputKey InputKeys::KEYBOARD_H_KEY = GLFW_KEY_H;
const InputKey InputKeys::KEYBOARD_I_KEY = GLFW_KEY_I;
const InputKey InputKeys::KEYBOARD_J_KEY = GLFW_KEY_J;
const InputKey InputKeys::KEYBOARD_K_KEY = GLFW_KEY_K;
const InputKey InputKeys::KEYBOARD_L_KEY = GLFW_KEY_L;
const InputKey InputKeys::KEYBOARD_M_KEY = GLFW_KEY_M;
const InputKey InputKeys::KEYBOARD_N_KEY = GLFW_KEY_N;
const InputKey InputKeys::KEYBOARD_O_KEY = GLFW_KEY_O;
const InputKey InputKeys::KEYBOARD_P_KEY = GLFW_KEY_P;
const InputKey InputKeys::KEYBOARD_Q_KEY = GLFW_KEY_Q;
const InputKey InputKeys::KEYBOARD_R_KEY = GLFW_KEY_R;
const InputKey InputKeys::KEYBOARD_S_KEY = GLFW_KEY_S;
const InputKey InputKeys::KEYBOARD_T_KEY = GLFW_KEY_T;
const InputKey InputKeys::KEYBOARD_U_KEY = GLFW_KEY_U;
const InputKey InputKeys::KEYBOARD_V_KEY = GLFW_KEY_V;
const InputKey InputKeys::KEYBOARD_W_KEY = GLFW_KEY_W;
const InputKey InputKeys::KEYBOARD_X_KEY = GLFW_KEY_X;
const InputKey InputKeys::KEYBOARD_Y_KEY = GLFW_KEY_Y;
const InputKey InputKeys::KEYBOARD_Z_KEY = GLFW_KEY_Z;

//Misc Keys
const InputKey InputKeys::KEYBOARD_SPACE = GLFW_KEY_SPACE;
const InputKey InputKeys::KEYBOARD_ESC = GLFW_KEY_ESCAPE;
const InputKey InputKeys::KEYBOARD_ENTER = GLFW_KEY_ENTER;
const InputKey InputKeys::KEYBOARD_TAB = GLFW_KEY_TAB;
const InputKey InputKeys::KEYBOARD_ACCENT = GLFW_KEY_GRAVE_ACCENT;

//Mouse Buttons (Add Mouse offset to all values!!)
const InputKey InputKeys::LEFT_MOUSE_BUTTON = GLFW_MOUSE_BUTTON_LEFT + MOUSE_OFFSET;
const InputKey InputKeys::RIGHT_MOUSE_BUTTON = GLFW_MOUSE_BUTTON_RIGHT + MOUSE_OFFSET;


bool InputKeys::IsKeyBoard(const InputKey key)
{
    return key < MOUSE_OFFSET;
}

bool InputKeys::IsMouse(const InputKey key)
{
    return key< GAMEPAD_OFFSET && key >= MOUSE_OFFSET;
}

bool InputKeys::IsGamePad(const InputKey key)
{
    return key >= GAMEPAD_OFFSET;
}


