#include "MeshRenderer.h"
#include "MeshComponent.h"
#include <Core/SceneTransformComponents.h>
#include <Rendering/RenderDataComponents.h>
#include <Camera/CameraComponent.h>
#include <Resources/Shader.h>

void MeshRenderer::Draw(World* world)
{
	int currentVAO = -1;
	int currentProgram = -1;
	int currentNumberOfIndices = 0;

	world->ForEntities<MeshComponent, SceneTransformComponent>
		(
			[&](const Entity entity, MeshComponent* mesh, SceneTransformComponent* transform)
			{
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

				glm::mat4 model = transform->GetTransformMatrix();

				if (RenderScale* scale = world->GetComponent<RenderScale>(entity))
				{
					model = glm::scale(model, glm::vec3(scale->scale.x, scale->scale.y, scale->scale.z));
				}
				SetShaderMatrix4(mat, "model", model);

				glDrawElements(GL_TRIANGLES, currentNumberOfIndices, GL_UNSIGNED_INT, 0);
			}
		);
}
