#include "CameraComponent.h"
#include  <Maths/Trigonometry.h>

#include <Platform/GameWindow.h>

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

glm::mat4 CameraComponent::GetProjection(const GameWindow* window) const
{
	if (projectionType == ProjectionType::PERSPECTIVE)
	{
		return glm::perspective(DegreesToRadians(fov), window->GetAspectRatio(), nearPlane, farPlane);
	}
	else
	{
		float doubleFov = fov * 2;
		float windowWidth = (float)window->GetWidth()/ doubleFov;
		float windowHeight = (float)window->GetHeight() / doubleFov;

		return glm::ortho(-windowWidth,windowWidth,-windowHeight,windowHeight, nearPlane, farPlane);
	}

}

glm::mat4 CameraComponent::GetView(Transform* transform) const
{
	Vector3 eye = transform->GetPosition();
	Vector3 center = transform->GetPosition() + transform->GetForward();
	Vector3 up = transform->GetUp();

	return glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(center.x, center.y, center.z), glm::vec3(up.x, up.y, up.z));
}

float CameraComponent::GetHorizontalFOV(const float aspectRatio) const
{
	float radAngle = DegreesToRadians(fov);
	return  2.f * atan(tan(radAngle / 2.f) * aspectRatio);
}

Vector3 CameraComponent::ScreenSpaceLocationToWorldSpaceDirection(Vector2 ScreenPosition, Transform* transform, GameWindow* gameWindow) const
{

	glm::vec4 rayClip = glm::vec4(ScreenPosition.x, ScreenPosition.y, -1.0f, 1.0f);

	glm::mat4 projection = GetProjection(gameWindow);
	glm::mat4 view = GetView(transform);

	glm::vec4 rayEye = glm::inverse(projection) * rayClip;

	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

	glm::vec4 rayWorld = glm::inverse(view) * rayEye;

	Vector3 toReturn = Vector3(rayWorld.x, rayWorld.y, rayWorld.z);


	return Vector3::Normalize(toReturn);
}
