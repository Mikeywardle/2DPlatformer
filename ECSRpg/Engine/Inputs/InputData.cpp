#include "InputData.h"

#include "InputValues.h"

InputData::InputData(const int NumberOfButtons)
{
    const int BufferLength = (NumberOfButtons * InputTypes::MAX_NUMBER_OF_TYPES);

    DataBuffer = new bool[BufferLength]();
}

InputData::~InputData()
{
    delete(DataBuffer);
}

bool InputData::GetInputValue(const int BoundButtonId, const InputType inputType) const
{
    const int BufferIndex = GetBufferIndex(BoundButtonId, inputType);

    return DataBuffer[BufferIndex];
}

void InputData::SetInputValue(const int BoundButtonId, const InputType inputType, const bool inputValue)
{
    const int BufferIndex = GetBufferIndex(BoundButtonId, inputType);

    DataBuffer[BufferIndex] = inputValue;
}

int InputData::GetBufferIndex(const int BoundButtonId, const InputType inputType) const
{
    return (BoundButtonId * InputTypes::MAX_NUMBER_OF_TYPES) + inputType;
}
