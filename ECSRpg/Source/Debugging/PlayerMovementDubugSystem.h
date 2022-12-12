#pragma once

#include <Debugging/IDebuggingSystem.h>

class PlayerMovementDebugSystem : public IDebuggingSystem
{
public:
	virtual void OnDebugUpdate(World* world, float deltaTime) override;
};

