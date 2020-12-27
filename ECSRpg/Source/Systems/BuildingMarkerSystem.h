#pragma once

#include <ecs/System.h>
#include <Inputs/InputReceiver.h>

class BuildingMarkerSystem : public System
{

public:
	BuildingMarkerSystem(World* world) : System(world) 
	{
		inputReceiver = InputReceiver();

		inputReceiver.AddMousePositionCallback(this, &BuildingMarkerSystem::OnMousePositionChanged);
	}

	virtual void OnFrame(float deltaTime) override {};

private:
	InputReceiver inputReceiver;

	void OnMousePositionChanged(Vector2 Position, Vector2 Delta);
};
