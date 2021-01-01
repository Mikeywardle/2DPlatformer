#pragma once
#include <string>
#include <vector>

#include "InputConfiguration.h"
#include "InputValues.h"

#include <Events/Event.h>
#include <Maths/Vector2.h>

#include <map>


namespace Inputs 
{
	extern std::map<InputKey, Event<InputKey, InputType>> PRESSED_BUTTON_MAPPINGS;
	extern std::map<InputKey, Event<InputKey, InputType>> RELEASED_BUTTON_MAPPINGS;

	extern Event<Vector2, Vector2> ON_MOUSE_POSITION_CHANGED;

	struct InputBinding
	{
		InputBinding() {}
		InputBinding(std::string name, InputType type, std::vector<InputKey> boundKeys, std::vector<EventDelegate<InputKey, InputType>*> delegates);

		std::string name;
		InputType type;
		std::vector<InputKey> boundKeys;
		std::vector<EventDelegate<InputKey, InputType>*> delegates;

		unsigned int numberOfBoundKeys;

		bool operator ==(InputBinding b);
	};

	void InitializeInputs(GLFWwindow* gameWindow);

	void SetInputConfiguration(InputConfiguration config);

	ButtonConfig FindButtonConfigByName(std::string name);

	template<typename Clazz, typename Function>
	InputBinding AddButtonMappingsToMap(Clazz* object,
		Function function,
		std::map<InputKey, Event<InputKey, InputType>>& bindingsMap,
		ButtonConfig config,
		std::string name,
		InputType pressedType)
	{
		std::vector<EventDelegate<InputKey, InputType>*> delegates;

		for (InputKey key : config.keys)
		{
			delegates.push_back(bindingsMap[key].AddListener(object, function));
		}


		return InputBinding(name, pressedType, config.keys, delegates);
	}

	template<typename Clazz, typename Function>
	InputBinding BindButton(Clazz* object, Function function, std::string name, InputType pressedType)
	{
		ButtonConfig config = FindButtonConfigByName(name);

		if (pressedType == InputTypes::BUTTON_PRESSED)
			return AddButtonMappingsToMap(object, function, PRESSED_BUTTON_MAPPINGS, config, name, pressedType);
		else
			return AddButtonMappingsToMap(object, function, RELEASED_BUTTON_MAPPINGS, config, name, pressedType);
	}

	void UnBindButton(InputKey key, EventDelegate<InputKey, InputType>* delegate, InputType type);


	template<typename Clazz, typename Function>
	EventDelegate<Vector2, Vector2>* BindMousePositionCallback(Clazz* object, Function function)
	{
		return ON_MOUSE_POSITION_CHANGED.AddListener(object, function);
	}

	void UnbindMousePositionCallback(EventDelegate<Vector2, Vector2>* delegate);

	Vector2 GetMousePosition();

	bool GetKeyPressed(InputKey key);

	bool GetBindingPressed(std::string binding);
}