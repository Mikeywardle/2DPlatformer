#pragma once

#include <Maths/MathsTypes.h>

typedef uint8 InputType;

struct InputTypes
{
public:
	static const InputType BUTTON_IS_DOWN;
	static const InputType BUTTON_RELEASED;
	static const InputType BUTTON_PRESSED;

	//Always largest type +1
	static const InputType MAX_NUMBER_OF_TYPES;

private:
	InputTypes() = default;
};

typedef uint16 InputKey;

#define MOUSE_OFFSET  512
#define GAMEPAD_OFFSET 16 + MOUSE_OFFSET

struct InputKeys
{
public:

	static bool IsKeyBoard(const InputKey key);
	static bool IsGamePad(const InputKey key);
	static bool IsMouse(const InputKey key);
public:
	//Letter Keys
	static const InputKey KEYBOARD_A_KEY;
	static const InputKey KEYBOARD_B_KEY;
	static const InputKey KEYBOARD_C_KEY;
	static const InputKey KEYBOARD_D_KEY;
	static const InputKey KEYBOARD_E_KEY;
	static const InputKey KEYBOARD_F_KEY;
	static const InputKey KEYBOARD_G_KEY;
	static const InputKey KEYBOARD_H_KEY;
	static const InputKey KEYBOARD_I_KEY;
	static const InputKey KEYBOARD_J_KEY;
	static const InputKey KEYBOARD_K_KEY;
	static const InputKey KEYBOARD_L_KEY;
	static const InputKey KEYBOARD_M_KEY;
	static const InputKey KEYBOARD_N_KEY;
	static const InputKey KEYBOARD_O_KEY;
	static const InputKey KEYBOARD_P_KEY;
	static const InputKey KEYBOARD_Q_KEY;
	static const InputKey KEYBOARD_R_KEY;
	static const InputKey KEYBOARD_S_KEY;
	static const InputKey KEYBOARD_T_KEY;
	static const InputKey KEYBOARD_U_KEY;
	static const InputKey KEYBOARD_V_KEY;
	static const InputKey KEYBOARD_W_KEY;
	static const InputKey KEYBOARD_X_KEY;
	static const InputKey KEYBOARD_Y_KEY;
	static const InputKey KEYBOARD_Z_KEY;

	//Misc Keys
	static const InputKey KEYBOARD_SPACE;
	static const InputKey KEYBOARD_ESC;
	static const InputKey KEYBOARD_ENTER;
	static const InputKey KEYBOARD_TAB;
	static const InputKey KEYBOARD_ACCENT;

	//Mouse Buttons (Add Mouse offset to all values!!)
	static const InputKey LEFT_MOUSE_BUTTON;
	static const InputKey RIGHT_MOUSE_BUTTON;

private:
	InputKeys() = default;
};