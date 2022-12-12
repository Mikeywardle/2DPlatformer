#include <Game.h>

#include <Core/World.h>
#include <Levels/MainMenuLevel.h>

#include "Source/Config/TestInputConfig.h"
#include <Config/NeOniPhysicsConfig.h>

#include <Debugging/DebugSystemsSetup.h>

int main()
{
	GameContext game = GameContext(TestInputConfig, NeOniPhysicsConfig);

	World* world = game.GetGameWorld();
	world->SwitchLevel<MainMenuLevel>();

	SetupDebugSystems(world);

	return game.StartGame();
}