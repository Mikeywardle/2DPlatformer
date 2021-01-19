#pragma once

#include <vector>

#include "PostProcessing/Quad.h"
#include "Skybox/SkyBox.h"

class World;
class CameraUniformBufferHandler;
class DirectionalLightBufferHandler;

class RenderingSystem
{
public:

	RenderingSystem(World* world);
	~RenderingSystem();

	void DrawWorld();

private:

	std::vector<class IRenderer*> renderers;

	World* world;
	CameraUniformBufferHandler* cameraUniformBufferHandler;
	DirectionalLightBufferHandler* directionalLightBufferHandler;

	//Test FrameBuffer
	unsigned int fbo;
	unsigned int buffertexture;
	unsigned int depthBuffertexture;
	unsigned int PosProcessingProgram;

	PostProcessQuad quad = PostProcessQuad();
	SkyBox skybox;
};
