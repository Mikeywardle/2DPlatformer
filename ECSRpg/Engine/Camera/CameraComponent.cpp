#include "CameraComponent.h"
#include  <Maths/Trigonometry.h>
#include <Game.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Entity CameraComponent::mainCamera;


Entity CameraComponent::GetMainCamera()
{
	return mainCamera;
}

void CameraComponent::SetMainCamera(Entity cameraEntity)
{
	mainCamera = cameraEntity;
}

glm::mat4 CameraComponent::GetProjection()
{
	if (projectionType == ProjectionType::PERSPECTIVE)
	{
		return glm::perspective(DegreesToRadians(fov), ASPECT_RATIO, nearPlane, farPlane);
	}
	else
	{
		float doubleFov = fov * 2;
		float windowWidth = (float)WINDOW_WIDTH/ doubleFov;
		float windowHeight = (float)WINDOW_HEIGHT / doubleFov;

		return glm::ortho(-windowWidth,windowWidth,-windowHeight,windowHeight, nearPlane, farPlane);
	}

}

glm::mat4 CameraComponent::GetView(Transform* transform)
{
	Vector3 eye = transform->GetPosition();
	Vector3 center = transform->GetPosition() + transform->GetForward();
	Vector3 up = transform->GetUp();

	return glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(center.x, center.y, center.z), glm::vec3(up.x, up.y, up.z));
}

float CameraComponent::GetHorizontalFOV()
{
	float radAngle = DegreesToRadians(fov);
	return  2.f * atan(tan(radAngle / 2.f) * ASPECT_RATIO);
}

Vector3 CameraComponent::ScreenPositionToWorldPosition(float xPos, float yPos, float depth, Transform* transform)
{
	if (projectionType == ProjectionType::PERSPECTIVE)
		return ScreenPositionToWorldPosition_Perspective(xPos, yPos, depth, transform);
	else
		return ScreenPositionToWorldPosition_Ortho(xPos, yPos, depth, transform);
}

Vector3 CameraComponent::ScreenPositionToWorldPosition_Ortho(float xPos, float yPos, float depth, Transform* transform)
{
	const float doubleFov = fov * 2;

	const float windowWidth = (float)WINDOW_WIDTH / doubleFov;
	const float windowHeight = (float)WINDOW_HEIGHT / doubleFov;

	const float xPosRatio = -(xPos-HALF_WINDOW_WIDTH)/HALF_WINDOW_WIDTH;
	const float yPosRatio = -(yPos-HALF_WINDOW_HEIGHT)/HALF_WINDOW_HEIGHT;

	const Vector3 CameraPosition = transform->GetPosition();

	return Vector3
	(
		CameraPosition.x + (xPosRatio * windowWidth),
		CameraPosition.y + (yPosRatio * windowHeight),
		depth
	);
}

Vector3 CameraComponent::ScreenPositionToWorldPosition_Perspective(float xPos, float yPos, float depth, Transform* transform)
{
	return Vector3();
}
