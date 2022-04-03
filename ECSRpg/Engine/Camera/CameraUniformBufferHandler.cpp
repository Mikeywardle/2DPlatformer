#include "CameraUniformBufferHandler.h"

#include <Core/World.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <Maths/Vector3.h>

#include <Camera/CameraComponent.h>
#include <Core/SceneTransformComponents.h>
#include <Rendering/UniformBufferLocations.h>

CameraUniformBufferHandler::CameraUniformBufferHandler()
{
	//Generate CameraUniform
	glGenBuffers(1, &cameraMatricesID);

	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatricesID);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, CAMERA_MATRICES_LOCATION);

	glBindBufferRange(GL_UNIFORM_BUFFER, CAMERA_MATRICES_LOCATION , cameraMatricesID, 0, 3 * sizeof(glm::mat4));

	//Generate Eye position Buffer
	glGenBuffers(1, &eyePositionID);

	glBindBuffer(GL_UNIFORM_BUFFER, eyePositionID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Vector3), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, EYE_POSITION_LOCATION);

	glBindBufferRange(GL_UNIFORM_BUFFER, EYE_POSITION_LOCATION, eyePositionID, 0, sizeof(Vector3));
}

void CameraUniformBufferHandler::BindCameraBuffers(World* world)
{
	Entity mainCamera = CameraComponent::GetMainCamera();

	if (!world->HasComponent<CameraComponent>(mainCamera))
		return;


	CameraComponent* camera = world->GetComponent<CameraComponent>(mainCamera);
	SceneTransformComponent* transform = world->GetComponent<SceneTransformComponent>(mainCamera);

	//Matrices
	glm::mat4 projection = camera->GetProjection(world->GetGameWindow());
	glm::mat4 view = camera->GetView(transform);
	glm::mat4 transformlessView = glm::mat4(glm::mat3(view));

	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatricesID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(transformlessView));

	//Eye Position
	Vector3 eyePosition = transform->GetPosition();

	glBindBuffer(GL_UNIFORM_BUFFER, eyePositionID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Vector3), &eyePosition);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
