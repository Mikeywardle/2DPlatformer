#pragma once

struct GLFWwindow;
struct InputData;

class GameWindow
{
public:

	GameWindow();
	~GameWindow();

	void RefereshScreen();
	//InputData PollInputs();

public:

	static GameWindow* CurrentWindow;

private:

	GLFWwindow* glfwWindow;

	int windowWidth;
	int windowHeight;
};
