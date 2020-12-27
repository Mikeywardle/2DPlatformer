#pragma once

struct Bitset
{
public:
	unsigned int capacity;

	Bitset(unsigned int capacity);

	void SetBit(int bit, bool value);
	bool GetBit(int bit) const;

	bool operator == (Bitset b);

private:
	char* dataBlock;
};
