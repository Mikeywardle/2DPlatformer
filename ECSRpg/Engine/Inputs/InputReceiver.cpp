#include "InputReceiver.h"
#include <Utils/STLUtils.h>
#include <Game.h>

InputReceiver::~InputReceiver()
{
	ClearBindings();
	RemoveMousePositionBinding();
}

void InputReceiver::RemoveBinding(Inputs::InputBinding toRemove)
{
	RemoveElementFromVector(inputBindings, toRemove);

	UnbindAllKeysFromBinding(toRemove);
}

void InputReceiver::RemoveMousePositionBinding()
{
	if (OnMousePositionChangedDelegate != nullptr)
		Inputs::UnbindMousePositionCallback(OnMousePositionChangedDelegate);
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

Vector2 InputReceiver::GetNormalizedMousePosition()
{
	const Vector2 MousePixelPosition = Inputs::GetMousePosition();

	const Vector2 NormalizedMousePosition = Vector2
	(
		(2.0f * MousePixelPosition.x) / (float)WINDOW_WIDTH - 1.0f
		, 1.0f - (2.0f * MousePixelPosition.y) / (float)WINDOW_HEIGHT
	);

	return NormalizedMousePosition;
}
