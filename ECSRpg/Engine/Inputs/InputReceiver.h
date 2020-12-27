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
	InputBinding AddButtonBinding(Clazz* object, Function function, std::string name, InputType pressedType)
	{
		InputBinding toReturn = BindButton(object, function, name, pressedType);
		inputBindings.push_back(toReturn);
		return toReturn;
	}

	template<typename Clazz, typename Function>
	void AddMousePositionCallback(Clazz* object, Function function)
	{
		OnMousePositionChangedDelegate = BindMousePositionCallback(object, function);
	}

	void RemoveBinding(InputBinding toRemove);

	void ClearBindings();

private:
	void UnbindAllKeysFromBinding(InputBinding& toRemove);
	std::vector<InputBinding> inputBindings;

	EventDelegate<Vector2, Vector2>* OnMousePositionChangedDelegate = nullptr;

};
