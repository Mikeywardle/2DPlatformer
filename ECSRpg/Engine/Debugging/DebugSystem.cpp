#include "DebugSystem.h"

#include <Game.h>

#include <Core/World.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <Inputs/InputReceiver.h>
#include <Inputs/InputValues.h>

#include <Rendering/Sprites/BillBoardSprites.h>
#include "../../Source/Components/PlayerMovementComponent.h"


DebugSystem::DebugSystem(World* world)
{
	this->world = world;

	inputReceiver = new InputReceiver();

	//TODO: Move to debug System file
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(GameWindow, true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

DebugSystem::~DebugSystem()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void DebugSystem::OnFrame(float deltaTime)
{
	bool TabPressed = inputReceiver->KeyPressed(InputKeys::KEYBOARD_TAB);
	if (TabPressed && !WasPressed)
		ToggleDebug();

	WasPressed = TabPressed;

	if (ShowDebug)
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Debug Tool");

		ImGui::Text("Frame Time:");
		ImGui::Text("%.2f fps", 1 / deltaTime);
		ImGui::Text("%.2f ms", deltaTime*1000);


		ImGui::End();



		ImGui::Begin("Player Tool");

		Entity player = world->GetEntities<PlayerMovementComponent, BillBoardComponent>()[0];
		Transform* t = world->GetComponent<Transform>(player);

		ImGui::Text("Rotation: %.2f", t->GetRotation().y);
		ImGui::End();

		//Draw ImGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


}

void DebugSystem::ToggleDebug()
{
	ShowDebug = !ShowDebug;
}
