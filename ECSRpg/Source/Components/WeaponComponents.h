#pragma once

struct PlayerWeaponComponent
{
public:

	PlayerWeaponComponent() = default;
	PlayerWeaponComponent(float firingRate);

	void ResetTimeToFire();

	float firingRate = 0.0f;
	float timeToFire = 0.0f;
};
