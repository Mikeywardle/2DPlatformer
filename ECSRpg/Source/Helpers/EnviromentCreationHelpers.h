#pragma once

#include <ecs/Entity.h>

struct Vector3;

class World;

namespace EnvironmentCreationHelpers
{
	Entity CreateGravityPanel(World* world, Vector3 Position, Vector3 rotation);

	Entity CreateFloatingPlatform(World* world
		, Vector3 position
		, Vector3 rotation
		, Vector3 scale
		, Vector3 desitnation);
}
