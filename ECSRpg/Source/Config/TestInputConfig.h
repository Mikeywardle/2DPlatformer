#pragma once

#include <Inputs/InputConfiguration.h>

static InputConfiguration TestConfig =
{
	//Buttons
	{
		{
			"Forwards",
			{InputKeys::KEYBOARD_W_KEY}
		},
		{
			"Backwards",
			{InputKeys::KEYBOARD_S_KEY}
		},
		{
			"Left",
			{InputKeys::KEYBOARD_A_KEY}
		},
		{
			"Right",
			{InputKeys::KEYBOARD_D_KEY}
		},
		{
			"Select",
			{InputKeys::LEFT_MOUSE_BUTTON}
		},
		{
			"Move",
			{InputKeys::RIGHT_MOUSE_BUTTON}
		},
		{
			"Jump",
			{InputKeys::KEYBOARD_SPACE}
		}

	}
};