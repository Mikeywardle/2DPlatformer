#pragma once

#include <ecs/Entity.h>

struct Vector3;

class World;

namespace EnvironmentCreationHelpers
{
	Entity CreateFixedCamera(World* world, const Vector3& position);
	Entity CreateGravityPanel(World* world, Vector3 Position, Vector3 rotation);

	Entity CreateFloatingPlatform(World* world
		, Vector3 position
		, Vector3 rotation
		, Vector3 scale
		, Vector3 desitnation);
}
