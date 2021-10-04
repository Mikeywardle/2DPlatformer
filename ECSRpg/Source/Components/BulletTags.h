#pragma once

#include <Maths/Vector3.h>
struct PlayerBullet {};

struct EnemyBullet {};

struct BulletComponent
{
	short damage = 0.0f;
};


struct BulletMovementComponent
{
	Vector3 velocity;
};