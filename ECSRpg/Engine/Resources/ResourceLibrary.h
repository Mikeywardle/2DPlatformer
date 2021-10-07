#pragma once
#include <string>
#include <unordered_map>

template<typename T>
class ResourceLibrary
{
public:
	std::unordered_map<std::string, T> resourceTable = std::unordered_map<std::string, T>();

	virtual void ClearAssets() = 0;

protected:
};
