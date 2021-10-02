#pragma once

#include <Inputs/InputConfiguration.h>

#include <Utils/EngineDefines.h>

enum TestConfigInputId : int
{
	Up = 0
	,Down = 1
	, Left = 2
	,Right = 3
	, Left_click = 4
	, Right_click = 5
	, Jump = 6
	, Fire = 7
};

static InputConfiguration TestInputConfig =
{
	//Buttons
	{
		{
			TestConfigInputId::Up,
			{InputKeys::KEYBOARD_W_KEY}
		},
		{
			TestConfigInputId::Down,
			{InputKeys::KEYBOARD_S_KEY}
		},
		{
			TestConfigInputId::Left,
			{InputKeys::KEYBOARD_A_KEY}
		},
		{
			TestConfigInputId::Right,
			{InputKeys::KEYBOARD_D_KEY}
		},
		{
			TestConfigInputId::Left_click,
			{InputKeys::LEFT_MOUSE_BUTTON}
		},
		{
			TestConfigInputId::Right_click,
			{InputKeys::RIGHT_MOUSE_BUTTON}
		},
		{
			TestConfigInputId::Jump,
			{InputKeys::KEYBOARD_SPACE}
		},
		{
			TestConfigInputId::Fire,
			{InputKeys::LEFT_MOUSE_BUTTON}
		}
	}

};