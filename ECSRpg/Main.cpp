#include <Game.h>

#include <Core/World.h>
#include "Source//Levels/DungeonLevel.h"
#include "Source/Config/TestInputConfig.h"

#include <Debug/DebugSystemsSetup.h>


int main()
{
	GameContext game = GameContext(TestInputConfig);
	World* world = game.GetGameWorld();
	world->SwitchLevel<DungeonLevel>();

	SetupDebugSystems(world);

	return game.StartGame();

}