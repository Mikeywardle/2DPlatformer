#include "GameWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Inputs/InputConfiguration.h>
#include <Inputs/InputData.h>

#include <stdio.h>

#pragma region GLFW_CALLBACKS
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	GameWindow* gameWindow = (GameWindow*)glfwGetWindowUserPointer(window);

	if (gameWindow)
	{
		gameWindow->SetHeight(height);
		gameWindow->SetWidth(width);
	}
}

void MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	GameWindow* gameWindow = (GameWindow*)glfwGetWindowUserPointer(window);

	if (gameWindow)
	{
		gameWindow->UpdateMousePosition(xpos, ypos);
	}
}

#pragma endregion

GameWindow::GameWindow(InputConfiguration& InInputConfig)
{
	InitialiseInputs(InInputConfig);
	InitialiseWindow();
	BindWindowCallbacks();
}


GameWindow::~GameWindow()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();

	delete(inputConfig);
	delete(inputData);
}

GameContext* GameWindow::GetGame() const
{
    return gameContext;
}

int GameWindow::GetWidth() const
{
	return width;
}

int GameWindow::GetHeight() const
{
	return height;
}

int GameWindow::GetHalfWidth() const
{
	return halfWidth;
}

int GameWindow::GetHalfHeight() const
{
	return halfHeight;
}

float GameWindow::GetAspectRatio() const
{
	return aspectRatio;
}

double GameWindow::GetWindowLifetime() const
{
	return glfwGetTime();
}

GLFWwindow* GameWindow::GetWindow() const
{
	return glfwWindow;
}

const InputData* GameWindow::GetFrameInputs()
{
	//Check for button presses
	for (int i = 0; i < inputConfig->buttonMappings.size(); ++i)
	{
		const ButtonConfig button = inputConfig->buttonMappings[i];

		bool ButtonDown = false;

		for (InputKey key : button.keys)
		{
			if (InputKeys::IsKeyBoard(key))
			{
				ButtonDown = ButtonDown || glfwGetKey(glfwWindow, key) == GLFW_PRESS;
			}
			else if (InputKeys::IsMouse(key))
			{
				ButtonDown = ButtonDown || glfwGetMouseButton(glfwWindow, key - MOUSE_OFFSET) == GLFW_PRESS;
			}

		}

		const bool ButtonWasDown = inputData->GetInputValue(button.ButtonId, InputTypes::BUTTON_IS_DOWN);

		const bool ButtonPressed = ButtonDown && !ButtonWasDown;
		const bool ButtonReleased = !ButtonDown && ButtonWasDown;

		inputData->SetInputValue(button.ButtonId, InputTypes::BUTTON_PRESSED, ButtonPressed);
		inputData->SetInputValue(button.ButtonId, InputTypes::BUTTON_RELEASED, ButtonReleased);
		inputData->SetInputValue(button.ButtonId, InputTypes::BUTTON_IS_DOWN, ButtonDown);
	}


	//Update Mouse data
	if (MouseMovedThisFrame)
	{
		MouseMovedThisFrame = false;
	}
	else
	{
		inputData->MouseDelta = Vector2::Zero;
	}

	return inputData;
}


void GameWindow::SetWidth(const int InWidth)
{
	width = InWidth;
	halfWidth = width / 2;

	CalculateAspectRatio();
}

void GameWindow::CalculateAspectRatio()
{
	aspectRatio = (float)width / (float)height;
}

void GameWindow::InitialiseInputs(InputConfiguration& InInputConfig)
{
	inputData = new InputData(InInputConfig.buttonMappings.size());

	inputConfig = new InputConfiguration();
	inputConfig->buttonMappings = InInputConfig.buttonMappings;
}

void GameWindow::SetHeight(const int inHeight)
{
	height = inHeight;
	halfHeight = height / 2;

	CalculateAspectRatio();
}

void GameWindow::UpdateMousePosition(const double xpos, const double ypos)
{
	const Vector2 OldMousePosition = inputData->MousePosition;

	inputData->MousePosition = Vector2(
				(2.0f *xpos)/ ((float)(width)) - 1.0f
				, 1-((2.0f * ypos) / (float) height)
		);

	inputData->MouseDelta = inputData->MousePosition - OldMousePosition;
	MouseMovedThisFrame = true;

}

bool GameWindow::IsMouseVisible()
{
	return glfwGetInputMode(glfwWindow, GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
}

void GameWindow::SetMouseVisible(const bool isVisible)
{
	if (isVisible)
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void GameWindow::RefreshFrame()
{
	glfwPollEvents();
	glfwSwapBuffers(glfwWindow);
}

bool GameWindow::IsWindowCLosed()
{
	return glfwWindowShouldClose(glfwWindow);
}

bool GameWindow::IsKeyPressed(const InputKey key)
{
	if (InputKeys::IsKeyBoard(key))
	{
		return glfwGetKey(glfwWindow, key) == GLFW_PRESS;
	}
	else if (InputKeys::IsMouse(key))
	{
		return glfwGetMouseButton(glfwWindow, key - MOUSE_OFFSET) == GLFW_PRESS;
	}
}

void GameWindow::InitialiseWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindow = glfwCreateWindow(800, 600, "RPG", NULL, NULL);

	SetHeight(600);
	SetWidth(800);

	if (glfwWindow == NULL)
	{
		printf("Failed to create window");
		glfwTerminate();
		initialised = false;
		return;
	}

	glfwMakeContextCurrent(glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		initialised = false;
		return;
	}


	glfwSetWindowUserPointer(glfwWindow, (void*)this);

	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	initialised = true;
}

void GameWindow::BindWindowCallbacks()
{
	glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);
	glfwSetCursorPosCallback(glfwWindow, MousePositionCallback);
}
