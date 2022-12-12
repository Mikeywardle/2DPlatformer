#include "PlayerMovementDubugSystem.h"

#include <ImGui/imgui.h>

#include <Components/PlayerMovementComponent.h>
#include <Core/SceneTransformComponents.h>
#include <Physics/RigidBody.h>

void PlayerMovementDebugSystem::OnDebugUpdate(World* world, float deltaTime)
{
	ImGui::Begin("Player Movement Debug");

	Entity player = world->GetEntity<PlayerMovementComponent, RigidBodyComponent ,SceneTransformComponent>();

	if (player != NO_ENTITY)
	{
		const PlayerMovementComponent* pmc = world->GetComponent<PlayerMovementComponent>(player);
		const SceneTransformComponent* stc = world->GetComponent<SceneTransformComponent>(player);
		const RigidBodyComponent* rgb = world->GetComponent<RigidBodyComponent>(player);

		ImGui::Text("Player Stats");

		const Vector3 position = stc->GetPosition();
		const Vector3 up = stc->GetUp();
		const Vector3 right = stc->GetRight();
		const Vector3 forward = stc->GetForward();
		const Vector3 rotation = stc->GetRotation();

		ImGui::Text("Position: x = %.2f, y = %.2f, z = %.2f", position.x, position.y, position.z);
		ImGui::Text("Rotation: x = %.2f, y = %.2f, z = %.2f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("");
		ImGui::Text("Forward: x = %.2f, y = %.2f, z = %.2f", forward.x, forward.y, forward.z);
		ImGui::Text("Up: x = %.2f, y = %.2f, z = %.2f", up.x, up.y, up.z);
		ImGui::Text("Right: x = %.2f, y = %.2f, z = %.2f", right.x, right.y, right.z);

		const Vector3 velocity = rgb->velocity;

		ImGui::Text("Velocity: x = %.2f, y = %.2f, z = %.2f", velocity.x, velocity.y, velocity.z);

		ImGui::Text("Is OnGround %s",pmc->OnGround ? "True" : "False");
		ImGui::Text("Is ToJump %s",pmc->ToJump ? "True" : "False");
		ImGui::Text("NumberOfJumps %i",pmc->NumberOfJumps);
		
	}
	else
	{
		ImGui::Text("No Player found");
	}


	ImGui::End();
}