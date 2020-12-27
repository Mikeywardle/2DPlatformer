#include "InputReceiver.h"
#include <Utils/STLUtils.h>

InputReceiver::~InputReceiver()
{
	ClearBindings();

	if (OnMousePositionChangedDelegate != nullptr)
		UnbindMousePositionCallback(OnMousePositionChangedDelegate);
}

void InputReceiver::RemoveBinding(InputBinding toRemove)
{
	RemoveElementFromVector(inputBindings, toRemove);

	UnbindAllKeysFromBinding(toRemove);
}

void InputReceiver::UnbindAllKeysFromBinding(InputBinding& toRemove)
{
	for (int i = 0; i < toRemove.numberOfBoundKeys; ++i)
	{
		UnBindButton(toRemove.boundKeys[i], toRemove.delegates[i], toRemove.type);
	}
}

void InputReceiver::ClearBindings()
{
	for (InputBinding binding : inputBindings)
	{
		UnbindAllKeysFromBinding(binding);
	}
}
