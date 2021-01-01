#include <Game.h>

#include "Source//Levels/DungeonLevel.h"
#include "Source/Config/TestInputConfig.h"

#include <Maths/Vector3.h>
#include <Physics/Collisions/CollisionDetection.h>

int main()
{
	InitializeGame();
	SetGameInputConfiguration(TestConfig);

	SetDefaultLevel<DungeonLevel>();

	StartGame();

	return 0;
}