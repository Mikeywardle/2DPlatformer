#include "BitsSet.h"
#include <stdlib.h>

Bitset::Bitset(unsigned int capacity)
{
    this->capacity = capacity;

    unsigned int byteCapacity = capacity / 8;

    if (capacity % 8 != 0)
        byteCapacity += 1;

    dataBlock = (char*)calloc(byteCapacity, sizeof(char));
}

void Bitset::SetBit(int bit, bool value)
{
}

bool Bitset::GetBit(int bit) const
{
    char bitMask = 0;
    return false;
}