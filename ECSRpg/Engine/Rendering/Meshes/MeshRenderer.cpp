#include "MeshRenderer.h"
#include "MeshComponent.h"
#include <Core/Transform.h>
#include <Camera/CameraComponent.h>
#include <Resources/Shader.h>

void MeshRenderer::Draw(World* world)
{
	//TODO: Move Camera Matrix Logic to own function (once per frame)
	Entity cameraEntity = CameraComponent::GetMainCamera();

	Transform* cameraTransform = world->GetComponent<Transform>(cameraEntity);
	CameraComponent* mainCamera = world->GetComponent<CameraComponent>(cameraEntity);

	glm::mat4 view = mainCamera->GetView(cameraTransform);
	glm::mat4 projection = mainCamera->GetProjection();
	//------------------------------------------------------------------------------------------------

	std::vector<Entity> entities = world->GetEntities<MeshComponent, Transform>();

	glm::mat4 model;

	int currentVAO = -1;
	int currentProgram = -1;
	int currentNumberOfIndices = 0;

	for (Entity entity : entities)
	{
		MeshComponent* mesh = world->GetComponent<MeshComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);

		int mat = mesh->GetMaterialIndex();
		int vao = mesh->GetMeshVAO();

		if (currentProgram != mat)
		{
			glUseProgram(mat);

			SetShaderMatrix4(mat, "view", view);
			SetShaderMatrix4(mat, "projection", projection);

			currentProgram = mat;
		}

		if (currentVAO != vao)
		{
			glBindVertexArray(vao);
			currentVAO = vao;
			currentNumberOfIndices = mesh->GetNumberOfIndices();
		}

		model = transform->GetModel();
		SetShaderMatrix4(mat, "model", model);


		glDrawElements(GL_TRIANGLES, currentNumberOfIndices, GL_UNSIGNED_INT, 0);
	}
}
