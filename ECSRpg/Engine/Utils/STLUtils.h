#pragma once

#include <vector>

template<typename Clazz>
void RemoveElementFromVector(std::vector<Clazz>& vector, Clazz toRemove)
{
	for (int i = 0; i < vector.size(); i++)
	{
		Clazz currentClazz = vector[i];

		if (currentClazz == toRemove)
		{
			vector.erase(vector.begin() + i);
			return;
		}
	}
};
