#include "MeshRenderer.h"
#include "MeshComponent.h"
#include <Maths/Transform.h>
#include <Camera/CameraComponent.h>
#include <Resources/Shader.h>

void MeshRenderer::Draw(World* world)
{
	glm::mat4 model;

	int currentVAO = -1;
	int currentProgram = -1;
	int currentNumberOfIndices = 0;

	ForEntities(world, MeshComponent, Transform)
	{

		MeshComponent* mesh = world->GetComponent<MeshComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);

		int mat = mesh->GetMaterialIndex();
		int vao = mesh->GetMeshVAO();

		if (currentProgram != mat)
		{
			glUseProgram(mat);
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
