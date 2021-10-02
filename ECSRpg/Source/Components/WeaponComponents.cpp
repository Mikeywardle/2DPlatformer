#include "WeaponComponents.h"

PlayerWeaponComponent::PlayerWeaponComponent(float firingRate)
{
	this->firingRate = firingRate;
}

void PlayerWeaponComponent::ResetTimeToFire()
{
	timeToFire = firingRate;
}
