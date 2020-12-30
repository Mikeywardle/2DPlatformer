#include <Game.h>

#include "Source/Levels/CityBuilderLevel.h"
#include "Source/PhysicsTest/PhysicsTestLevel.h"
#include "Source/Config/TestInputConfig.h"

#include <Maths/Vector3.h>
#include <Physics/Collisions/CollisionDetection.h>

int main()
{
	InitializeGame();
	SetGameInputConfiguration(TestConfig);

	SetDefaultLevel<PhysicsTestLevel>();

	StartGame();

	return 0;
}