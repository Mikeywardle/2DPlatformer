#include "Game.h"

#include <stdio.h>

#include <Core/GameTime.h>
#include <Rendering/Sprites/Sprite.h>
#include <UI/Text.h>
#include<Inputs/Inputs.h>

GLFWwindow* GameWindow;

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

int HALF_WINDOW_WIDTH = WINDOW_WIDTH / 2;
int HALF_WINDOW_HEIGHT = WINDOW_HEIGHT / 2;
float ASPECT_RATIO;

World* GameWorld;


void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	HALF_WINDOW_WIDTH = WINDOW_WIDTH / 2;
	HALF_WINDOW_HEIGHT = WINDOW_HEIGHT / 2;

	ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
}

bool InitializeWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GameWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "RPG", NULL, NULL);
	ASPECT_RATIO = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

	if (GameWindow == NULL)
	{
		printf("Failed to create window");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(GameWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return false;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(GameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(0);

	return true;
}

void BindCallBackFunctions()
{
	glfwSetFramebufferSizeCallback(GameWindow, FramebufferSizeCallback);
}

void InitializeData()
{
	InitializeTextData();
}

void UpdateGameTime()
{
	double time = glfwGetTime();
	REAL_DELTA_TIME = (float)(time - TOTAL_GAME_TIME);
	DELTA_TIME = REAL_DELTA_TIME * TIME_SCALE;
	TOTAL_GAME_TIME = time;
}

void InitializeGame()
{
	bool initialized = InitializeWindow();
	if (!initialized)
	{
		printf("FAILED TO RUN!!");
		return;
	}

	BindCallBackFunctions();
	InitializeInputs(GameWindow);
	InitializeData();

	GameWorld = new World();
}

void SetGameInputConfiguration(InputConfiguration config)
{
	SetInputConfiguration(config);
}

int StartGame()
{
	glClearColor(0, 0, 0, 1.0f);

	while (!glfwWindowShouldClose(GameWindow))
	{

		glfwPollEvents();
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GameWorld->RunFrame(DELTA_TIME);
		glfwSwapBuffers(GameWindow);
		UpdateGameTime();	
	}
	glfwTerminate();

	return 0;
}

void EndGame()
{
	glfwSetWindowShouldClose(GameWindow, true);
}

