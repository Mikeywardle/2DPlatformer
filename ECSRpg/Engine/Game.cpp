#include "Game.h"

#include <stdio.h>

#include <Inputs/InputConfiguration.h>
#include <Inputs/InputData.h>

#include <Physics/PhysicsSystemConfig.h>

#include <Platform/GameWindow.h>
#include <Core/World.h>

GameContext::GameContext(InputConfiguration InputConfig, PhysicsSystemConfig physicsConfig)
{
	gameWindow = new GameWindow(InputConfig);
	gameWorld = new World(this, physicsConfig);
}

GameContext::~GameContext()
{
	delete(gameWindow);
	delete(gameWorld);
}

int GameContext::StartGame()
{
	while (!ShouldClose && !gameWindow->IsWindowCLosed())
	{
		gameWindow->RefreshFrame();
		const InputData* inputData = gameWindow->GetFrameInputs();
		gameWorld->RunFrame(LastDeltaTime, inputData);
		UpdateFrameTime();
	}

	return 0;
}

void GameContext::EndGame()
{
	ShouldClose = true;
}

GameWindow* GameContext::GetGameWindow() const
{
	return gameWindow;
}

World* GameContext::GetGameWorld() const
{
	return gameWorld;
}

void GameContext::UpdateFrameTime()
{
	//DELTA_TIME = REAL_DELTA_TIME * TIME_SCALE;


	const double time = gameWindow->GetWindowLifetime();
	LastDeltaTime = (float)(time - GameLifetime);
	GameLifetime = time;
}
