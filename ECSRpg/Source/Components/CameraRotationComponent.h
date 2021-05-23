#pragma once

struct CameraRotationComponent
{
	CameraRotationComponent() = default;
	CameraRotationComponent(float InRotationSpeed);

	float RotationSpeed;
};
