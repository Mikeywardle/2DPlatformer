#pragma once
#include <Core/Transform.h>
#include <glm/mat4x4.hpp>
#include <Maths/Vector3.h>
#include <ecs/Entity.h>

enum class ProjectionType
{
	PERSPECTIVE,
	ORTHOGRAPHIC
};

struct CameraComponent
{

public:

	static Entity GetMainCamera();
	static void SetMainCamera(Entity cameraEntity);

	ProjectionType projectionType;
	float fov;
	float nearPlane;
	float farPlane;

	glm::mat4 GetProjection();
	glm::mat4 GetView(Transform* transform);

	float GetHorizontalFOV();
	Vector3 ScreenPositionToWorldPosition(float xPos, float yPos, float depth, Transform* transform);

private:
	Vector3 ScreenPositionToWorldPosition_Ortho(float xPos, float yPos, float depth, Transform* transform);
	Vector3 ScreenPositionToWorldPosition_Perspective(float xPos, float yPos, float depth, Transform* transform);

	static Entity mainCamera;

};

