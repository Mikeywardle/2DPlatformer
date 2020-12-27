#pragma once
#include <GLFW/glfw3.h>

typedef int  InputType;

struct InputTypes
{
public:
	static const InputType BUTTON_PRESSED = 1;
	static const InputType BUTTON_RELEASED = 0;

private:
	InputTypes() {}
};

#define KEYBOARD_OFFSET  512
#define MOUSE_OFFSET  16 + KEYBOARD_OFFSET
#define GAMEPAD_OFFSET 32 + MOUSE_OFFSET

typedef int InputKey;

struct InputKeys
{
public:

	//Letter Keys
	static const InputKey KEYBOARD_A_KEY = GLFW_KEY_A;
	static const InputKey KEYBOARD_B_KEY = GLFW_KEY_B;
	static const InputKey  KEYBOARD_C_KEY = GLFW_KEY_C;
	static const InputKey  KEYBOARD_D_KEY = GLFW_KEY_D;
	static const InputKey  KEYBOARD_E_KEY = GLFW_KEY_E;
	static const InputKey  KEYBOARD_F_KEY = GLFW_KEY_F;
	static const InputKey  KEYBOARD_G_KEY = GLFW_KEY_G;
	static const InputKey  KEYBOARD_H_KEY = GLFW_KEY_H;
	static const InputKey  KEYBOARD_I_KEY = GLFW_KEY_I;
	static const InputKey  KEYBOARD_J_KEY = GLFW_KEY_J;
	static const InputKey  KEYBOARD_K_KEY = GLFW_KEY_K;
	static const InputKey  KEYBOARD_L_KEY = GLFW_KEY_L;
	static const InputKey  KEYBOARD_M_KEY = GLFW_KEY_M;
	static const InputKey  KEYBOARD_N_KEY = GLFW_KEY_N;
	static const InputKey  KEYBOARD_O_KEY = GLFW_KEY_O;
	static const InputKey  KEYBOARD_P_KEY = GLFW_KEY_P;
	static const InputKey  KEYBOARD_Q_KEY = GLFW_KEY_Q;
	static const InputKey  KEYBOARD_R_KEY = GLFW_KEY_R;
	static const InputKey  KEYBOARD_S_KEY = GLFW_KEY_S;
	static const InputKey  KEYBOARD_T_KEY = GLFW_KEY_T;
	static const InputKey  KEYBOARD_U_KEY = GLFW_KEY_U;
	static const InputKey  KEYBOARD_V_KEY = GLFW_KEY_V;
	static const InputKey  KEYBOARD_W_KEY = GLFW_KEY_W;
	static const InputKey  KEYBOARD_X_KEY = GLFW_KEY_X;
	static const InputKey  KEYBOARD_Y_KEY = GLFW_KEY_Y;
	static const InputKey  KEYBOARD_Z_KEY = GLFW_KEY_Z;

	//Misc Keys
	static const InputKey KEYBOARD_SPACE = GLFW_KEY_SPACE;
	static const InputKey KEYBOARD_ESC = GLFW_KEY_ESCAPE;
	static const InputKey KEYBOARD_ENTER = GLFW_KEY_ENTER;

	//Mouse Buttons (Add Mouse offset to all values!!)
	static const InputKey LEFT_MOUSE_BUTTON = GLFW_MOUSE_BUTTON_1 + MOUSE_OFFSET;
	static const InputKey RIGHT_MOUSE_BUTTON = GLFW_MOUSE_BUTTON_2 + MOUSE_OFFSET;

private:
	InputKeys() {}
};

