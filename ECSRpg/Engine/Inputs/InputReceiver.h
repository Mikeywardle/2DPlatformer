#pragma once

#include <Core/Behaviour.h>
#include <string>
#include <Inputs/InputValues.h>
#include <Inputs/Inputs.h>
#include <map>
#include <Events/Event.h>
#include <vector>


class InputReceiver
{
public:

	~InputReceiver();

	template<typename Clazz, typename Function>
	Inputs::InputBinding AddButtonBinding(Clazz* object, Function function, std::string name, InputType pressedType)
	{
		Inputs::InputBinding toReturn = Inputs::BindButton(object, function, name, pressedType);
		inputBindings.push_back(toReturn);
		return toReturn;
	}

	template<typename Clazz, typename Function>
	void AddMousePositionCallback(Clazz* object, Function function)
	{
		OnMousePositionChangedDelegate = Inputs::BindMousePositionCallback(object, function);
	}

	void RemoveBinding(Inputs::InputBinding toRemove);

	void ClearBindings();

	bool KeyPressed(InputKey key);

	bool BindingPressed(std::string binding);

private:
	void UnbindAllKeysFromBinding(Inputs::InputBinding& toRemove);
	std::vector<Inputs::InputBinding> inputBindings;

	EventDelegate<Vector2, Vector2>* OnMousePositionChangedDelegate = nullptr;

};
