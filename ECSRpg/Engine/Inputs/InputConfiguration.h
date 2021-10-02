#pragma once

#include "InputValues.h"

#include <vector>
#include <string>


struct ButtonConfig
{
	int ButtonId;
	std::vector<InputKey> keys;
};

struct InputConfiguration
{
	std::vector<ButtonConfig> buttonMappings;
};
