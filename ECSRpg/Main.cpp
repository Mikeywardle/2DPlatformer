#include <Game.h>

#include "Source//Levels/DungeonLevel.h"
#include "Source/Config/TestInputConfig.h"


int main()
{
	InitializeGame();
	SetGameInputConfiguration(TestConfig);

	SetDefaultLevel<DungeonLevel>();

	StartGame();

	return 0;
}