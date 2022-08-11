#pragma once

#include <ecs/Entity.h>

struct Vector3;

class World;

namespace EnvironmentCreationHelpers
{
	Entity CreateGravityPanel(World* world, Vector3 Position, Vector3 rotation);
}
