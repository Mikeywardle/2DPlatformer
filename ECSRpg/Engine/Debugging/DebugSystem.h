#pragma once

#include <vector>
#include "IDebuggingSystem.h"

class World;
class InputReceiver;
class IDebuggingSystem;

class DebugSystem
{
public:
	DebugSystem(World* world);
	~DebugSystem();

	void OnFrame(const float deltaTime, const InputData* inputData);

	template<typename T>
	void AddSystem()
	{

		T* newSystem = new T();
		debugSystems.push_back(dynamic_cast<IDebuggingSystem*>(newSystem));

	}


private:
	World* world;

	bool ShowDebug = false;
	bool TabWasPressed = false;
	bool TildaWasPressed = false;
	void ToggleDebug();

	InputReceiver* inputReceiver;

	std::vector<IDebuggingSystem*> debugSystems;
};
