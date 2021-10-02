#pragma once

#include <Maths/MathsTypes.h>

class GameContext;

struct InputData;
struct InputConfiguration;
typedef uint16 InputKey;
typedef struct GLFWwindow GLFWwindow;

class GameWindow
{

public:
	GameWindow(InputConfiguration& InInputConfig);
	~GameWindow();

	GameContext* GetGame() const;
	GLFWwindow* GetWindow() const;
	const InputData* GetFrameInputs();

	int GetWidth() const;
	int GetHeight() const;
	int GetHalfWidth() const;
	int GetHalfHeight() const;

	void SetWidth(const int InWidth);
	void SetHeight(const int inHeight);

	void UpdateMousePosition(const double xpos, const double ypos);

	bool IsMouseVisible();
	void SetMouseVisible(const bool isVisible);

	float GetAspectRatio() const;
	double GetWindowLifetime() const;

	void RefreshFrame();

	bool IsWindowCLosed();

	bool IsKeyPressed(const InputKey key);

private:

	void InitialiseWindow();
	void BindWindowCallbacks();
	void CalculateAspectRatio();
	void InitialiseInputs(InputConfiguration& InInputConfig);

private:

	GameContext* gameContext;
	GLFWwindow* glfwWindow;
	InputData* inputData;
	InputConfiguration* inputConfig;

	bool initialised = false;

	bool MouseMovedThisFrame;

	int width = 0;
	int height = 0;

	int halfWidth = 0;
	int halfHeight = 0;

	float aspectRatio = 0.0f;
};