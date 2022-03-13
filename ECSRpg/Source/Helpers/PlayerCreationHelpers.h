#pragma once

class World;

struct Vector3;

namespace PlayerCreation
{
	void SpawnPlayer(World* world, const Vector3& position);
	void SpawnDungeonPlayer(World* world, const Vector3& position);
}