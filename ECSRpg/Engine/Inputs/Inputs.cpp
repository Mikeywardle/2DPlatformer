#include "Inputs.h"

#include <GLFW/glfw3.h>

#include <set>
#include "InputValues.h"

std::map<InputKey, Event<InputKey, InputType>> PRESSED_BUTTON_MAPPINGS;
std::map<InputKey, Event<InputKey, InputType>> RELEASED_BUTTON_MAPPINGS;

Event<Vector2, Vector2> ON_MOUSE_POSITION_CHANGED;

std::set<InputKey> REGISTERED_KEYS;

InputConfiguration CURRENT_INPUT_CONFIGURATION;

Vector2 MOUSE_POSITION = Vector2(0,0);
Vector2 MOUSE_DELTA = Vector2(0, 0);

void InvokeKeyEvent(InputKey& inputKey, const InputType& type);

InputBinding::InputBinding(std::string name, InputType type, std::vector<InputKey> boundKeys, std::vector<EventDelegate<InputKey, InputType>*> delegates)
{
	this->name = name;
	this->type = type;
	this->boundKeys = boundKeys;
	this->delegates = delegates;

	numberOfBoundKeys = boundKeys.size();
}

bool InputBinding::operator==(InputBinding b)
{
	return name == b.name 
		&& numberOfBoundKeys == b.numberOfBoundKeys 
		&& type == b.type;
}

void KeyPressCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  	InputKey inputKey = (InputKey)key;
	InputType type = (InputType)action;

	InvokeKeyEvent(inputKey, type);

}
void MouseClickCallBack(GLFWwindow* window, int button, int action, int mods)
{

	InputKey inputKey = (InputKey)button + MOUSE_OFFSET;
	InputType type = (InputType)action;

	InvokeKeyEvent(inputKey, type);
}

void MousePositionCallBack(GLFWwindow* window, double xpos, double ypos)
{
	Vector2 OldMousePosition = MOUSE_POSITION;

	MOUSE_POSITION = Vector2(xpos, ypos);

	MOUSE_DELTA = MOUSE_POSITION - OldMousePosition;

	ON_MOUSE_POSITION_CHANGED.Invoke(MOUSE_POSITION, MOUSE_DELTA);
}

void InvokeKeyEvent(InputKey& inputKey, const InputType& type)
{
	if (REGISTERED_KEYS.find(inputKey) == REGISTERED_KEYS.end())
		return;

	if (type == InputTypes::BUTTON_PRESSED)
		PRESSED_BUTTON_MAPPINGS[inputKey].Invoke(inputKey, type);
	else if (type == InputTypes::BUTTON_RELEASED)
		RELEASED_BUTTON_MAPPINGS[inputKey].Invoke(inputKey, type);
}

void InitializeInputs(GLFWwindow* gameWindow)
{
	glfwSetKeyCallback(gameWindow, KeyPressCallBack);
	glfwSetMouseButtonCallback(gameWindow, MouseClickCallBack);
	glfwSetCursorPosCallback(gameWindow, MousePositionCallBack);

}

void SetInputConfiguration(InputConfiguration config)
{
	CURRENT_INPUT_CONFIGURATION = config;

	for (ButtonConfig buttonConfig : config.buttonMappings)
	{
		for (InputKey key : buttonConfig.keys)
		{
			PRESSED_BUTTON_MAPPINGS[key] = Event<InputKey, InputType>();
			RELEASED_BUTTON_MAPPINGS[key] = Event<InputKey, InputType>();
			REGISTERED_KEYS.insert(key);
		}
	}
}



void UnBindButton(InputKey key, EventDelegate<InputKey, InputType>* delegate, InputType type)
{
	if (type == InputTypes::BUTTON_PRESSED)
	{
		PRESSED_BUTTON_MAPPINGS[key].RemoveListener(delegate);
	}
	else
	{
		RELEASED_BUTTON_MAPPINGS[key].RemoveListener(delegate);
	}
}

ButtonConfig FindButtonConfigByName(std::string name)
{
	for (ButtonConfig config : CURRENT_INPUT_CONFIGURATION.buttonMappings)
	{
		if (config.name == name)
			return config;
	}

	return ButtonConfig();
}

void UnbindMousePositionCallback(EventDelegate<Vector2, Vector2>* delegate)
{
	ON_MOUSE_POSITION_CHANGED.RemoveListener(delegate);
}

Vector2 GetMousePosition()
{
	return MOUSE_POSITION;
}
