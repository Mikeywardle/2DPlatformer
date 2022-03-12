#pragma once

#include <Inputs/InputConfiguration.h>

#include <Utils/EngineDefines.h>

enum TestConfigInputId : int
{
	Up = 0
	, Down
	, Left
	, Right
	, Turn_Left
	, Turn_Right
	, Left_click
	, Right_click
	, Jump
	, Fire
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
			TestConfigInputId::Turn_Left,
			{InputKeys::KEYBOARD_Q_KEY}
		},
		{
			TestConfigInputId::Turn_Right,
			{InputKeys::KEYBOARD_E_KEY}
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