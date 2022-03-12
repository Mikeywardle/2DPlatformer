#include "MeshRenderer.h"
#include "MeshComponent.h"
#include <Maths/Transform.h>
#include <Camera/CameraComponent.h>
#include <Resources/Shader.h>

void MeshRenderer::Draw(World* world)
{
	int currentVAO = -1;
	int currentProgram = -1;
	int currentNumberOfIndices = 0;

	ForEntities(world, MeshComponent, Transform)
	{
		const MeshComponent* mesh = world->GetComponent<MeshComponent>(entity);
		Transform* transform = world->GetComponent<Transform>(entity);

		const int mat = mesh->GetMaterialIndex();
		const int vao = mesh->GetMeshVAO();

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

		const glm::mat4 model = transform->GetModel();
		SetShaderMatrix4(mat, "model", model);

		glDrawElements(GL_TRIANGLES, currentNumberOfIndices, GL_UNSIGNED_INT, 0);
	}

}
