#pragma once

#include <Maths/MathsTypes.h>
#include <Maths/Vector2.h>

typedef uint8 InputType;

struct InputData
{
public:
	InputData(const int NumberOfButtons);
	~InputData();

	bool GetInputValue(const int BoundButtonId, const InputType inputType) const;
	void SetInputValue(const int BoundButtonId, const InputType inputType, const bool inputValue);

public:
	Vector2 MousePosition = Vector2::Zero;
	Vector2 MouseDelta = Vector2::Zero;

private:

	int GetBufferIndex(const int BoundButtonId, const InputType inputType) const;

private:
	char* DataBuffer;
};
