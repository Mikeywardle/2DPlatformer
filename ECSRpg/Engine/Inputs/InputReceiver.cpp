#include "InputReceiver.h"
#include <Utils/STLUtils.h>

InputReceiver::~InputReceiver()
{
	ClearBindings();

	if (OnMousePositionChangedDelegate != nullptr)
		Inputs::UnbindMousePositionCallback(OnMousePositionChangedDelegate);
}

void InputReceiver::RemoveBinding(Inputs::InputBinding toRemove)
{
	RemoveElementFromVector(inputBindings, toRemove);

	UnbindAllKeysFromBinding(toRemove);
}

void InputReceiver::UnbindAllKeysFromBinding(Inputs::InputBinding& toRemove)
{
	for (int i = 0; i < toRemove.numberOfBoundKeys; ++i)
	{
		Inputs::UnBindButton(toRemove.boundKeys[i], toRemove.delegates[i], toRemove.type);
	}
}

void InputReceiver::ClearBindings()
{
	for (Inputs::InputBinding binding : inputBindings)
	{
		UnbindAllKeysFromBinding(binding);
	}
}

bool InputReceiver::KeyPressed(InputKey key)
{
	return Inputs::GetKeyPressed(key);
}

bool InputReceiver::BindingPressed(std::string binding)
{
	return Inputs::GetBindingPressed(binding);
}
