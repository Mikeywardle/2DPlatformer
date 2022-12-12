#pragma once

#include <Core/World.h>

#include <Debugging/DebugSystem.h>
#include <Utils/EngineDefines.h>

#include "PlayerMovementDubugSystem.h"

void SetupDebugSystems(World* world)
{
#if NOT_RELEASE_BUILD
	DebugSystem* debugSystem = world->GetDebugSystem();

	debugSystem->AddSystem<PlayerMovementDebugSystem>();
#endif
}