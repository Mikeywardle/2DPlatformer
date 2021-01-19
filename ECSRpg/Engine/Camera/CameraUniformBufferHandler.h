#pragma once

class CameraUniformBufferHandler
{
public:
	CameraUniformBufferHandler();

	void BindCameraBuffers(class World* world);

private:
	unsigned int cameraMatricesID;
	unsigned int eyePositionID;

};
