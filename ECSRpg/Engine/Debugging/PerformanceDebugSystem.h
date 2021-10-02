#pragma once

#include "IDebuggingSystem.h"

class PerformanceDebugSystem : public IDebuggingSystem
{
public:
	virtual void OnDebugUpdate(World* world, float deltaTime) override;
};
