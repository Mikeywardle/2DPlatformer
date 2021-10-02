#pragma once

class World;
struct Vector3;

namespace BulletCreation
{
	void SpawnPlayerBullet(World* world, Vector3 position, Vector3 heading);
}