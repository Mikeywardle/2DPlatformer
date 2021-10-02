#include "InputData.h"

#include "InputValues.h"

InputData::InputData(const int NumberOfButtons)
{
    const int BufferLength = ((NumberOfButtons * InputTypes::MAX_NUMBER_OF_TYPES)/8)+1;

    DataBuffer = new char[BufferLength];
}

InputData::~InputData()
{
    delete(DataBuffer);
}

bool InputData::GetInputValue(const int BoundButtonId, const InputType inputType) const
{
    const int BufferIndex = GetBufferIndex(BoundButtonId, inputType);
    const int byte = BufferIndex / 8;
    const int bit = BufferIndex % 8;

    return !!((DataBuffer[byte]) & (1 << (bit)));
}

void InputData::SetInputValue(const int BoundButtonId, const InputType inputType, const bool inputValue)
{
    const int BufferIndex = GetBufferIndex(BoundButtonId, inputType);

    const int byte = BufferIndex / 8;
    const int bit = BufferIndex % 8;

    if (inputValue)
    {
        DataBuffer[byte] |= (1UL << bit);
    }
    else
    {
        DataBuffer[byte] &= ~(1UL << bit);
    }

}

int InputData::GetBufferIndex(const int BoundButtonId, const InputType inputType) const
{
    return (BoundButtonId *InputTypes::MAX_NUMBER_OF_TYPES) + inputType;
}
