#pragma once

#include <Core/World.h>

struct InputConfiguration;
struct GLFWwindow;
//class World;

void InitializeGame();
void SetGameInputConfiguration(InputConfiguration config);
int StartGame();
void EndGame();


extern GLFWwindow* GameWindow;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int HALF_WINDOW_WIDTH;
extern int HALF_WINDOW_HEIGHT;
extern float ASPECT_RATIO;

extern World* GameWorld;


template <class T>
void SetDefaultLevel()
{
	GameWorld->SwitchLevel<T>();
}

class GameContext
{
private:
	World* gameWorld;
//	GameWindow* gameWindow;
};

