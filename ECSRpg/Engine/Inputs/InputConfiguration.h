#pragma once

#include "InputValues.h"
#include <vector>
#include <string>


struct ButtonConfig
{
	std::string name;
	std::vector<InputKey> keys;
};

struct InputConfiguration
{
	std::vector<ButtonConfig> buttonMappings;
};
