#pragma once

class World;
class InputReceiver;

class DebugSystem
{
public:
	DebugSystem(World* world);
	~DebugSystem();

	void OnFrame(float deltaTime);

private:
	World* world;

	bool ShowDebug = false;
	bool WasPressed = false;
	void ToggleDebug();

	InputReceiver* inputReceiver;
};
