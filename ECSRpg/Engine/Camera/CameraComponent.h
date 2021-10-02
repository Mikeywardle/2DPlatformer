#pragma once
#include <Maths/Transform.h>
#include <glm/mat4x4.hpp>
#include <Maths/Vector3.h>
#include <Maths/Vector2.h>
#include <ecs/Entity.h>

class GameWindow;

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

	glm::mat4 GetProjection(const GameWindow* window) const;
	glm::mat4 GetView(Transform* transform) const;

	float GetHorizontalFOV(const float aspectRatio) const;
	Vector3 ScreenSpaceLocationToWorldSpaceDirection(Vector2 ScreenPosition, Transform* transform, GameWindow* gameWindow) const;

private:
	static Entity mainCamera;

};

