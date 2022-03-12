#include "DebugSystem.h"

#include <Core/World.h>

#include <Platform/GameWindow.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <Inputs/InputValues.h>
#include <Inputs/InputData.h>

#include "PerformanceDebugSystem.h"
#include "CollisionDebugSystem.h"

#include "DebugDraws.h"

DebugSystem::DebugSystem(World* world)
{
	this->world = world;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(world->GetGameWindow()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");

	AddSystem<PerformanceDebugSystem>();
	AddSystem<CollisionDebugSystem>();

	DebugDraws::InitialiseDebugShader();
}

DebugSystem::~DebugSystem()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void DebugSystem::OnFrame(const float deltaTime , const InputData* inputData)
{
	GameWindow* window = world->GetGameWindow();
	bool TabPressed = window->IsKeyPressed(InputKeys::KEYBOARD_TAB);

	if (TabPressed && !TabWasPressed)
		ToggleDebug();

	bool TildaPressed = window->IsKeyPressed(InputKeys::KEYBOARD_ACCENT);

	if (TildaPressed && !TildaWasPressed)
	{
		const bool isMouseVisible = !window->IsMouseVisible();
		window->SetMouseVisible(isMouseVisible);
	}

	TabWasPressed = TabPressed;
	TildaWasPressed = TildaPressed;

	if (ShowDebug)
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (IDebuggingSystem* System : debugSystems)
		{
			System->OnDebugUpdate(world, deltaTime);
		}

		//Draw ImGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void DebugSystem::ToggleDebug()
{
	ShowDebug = !ShowDebug;
}
