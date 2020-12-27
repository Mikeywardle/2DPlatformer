#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/World.h>
#include <Inputs/InputConfiguration.h>

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

