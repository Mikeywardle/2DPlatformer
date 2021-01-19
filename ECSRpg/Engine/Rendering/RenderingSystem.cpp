#include "RenderingSystem.h"

#include "IRenderer.h"
#include "Sprites/SpriteRenderer.h"
#include "Meshes/MeshRenderer.h"

#include <Camera/CameraUniformBufferHandler.h>

#include <Lighting/UniformBufferHandlers/DirectionalLightBufferHandler.h>

#include <Core/World.h>
#include <Resources/Shader.h>

#include <iostream>

#include <stb_image.h>

RenderingSystem::RenderingSystem(World* world)
{
	this->world = world;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);


	//Enable Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glClearColor(0, 0, 0, 1.0f);

	//TODO: Move to.... somewhere 
	{
		//Test FrameBuffer
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		//TestBuffer Texture
		glGenTextures(1, &buffertexture);
		glBindTexture(GL_TEXTURE_2D, buffertexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffertexture, 0);

		//Depth texture
		glGenTextures(1, &depthBuffertexture);
		glBindTexture(GL_TEXTURE_2D, depthBuffertexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffertexture, 0);

		////Render Buffer object
		//unsigned int rbo;
		//glGenRenderbuffers(1, &rbo);
		//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		//Load and create Shaders
		GLuint ppFrag = LoadShaderFromFile("Resources/Shaders/postProcessFrag.fs", GL_FRAGMENT_SHADER);
		GLuint ppVert = LoadShaderFromFile("Resources/Shaders/postProcessVert.vs", GL_VERTEX_SHADER);

		PosProcessingProgram = CreateProgram(ppVert, ppFrag);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//SkyBox
	{
		GLuint skyFrag = LoadShaderFromFile("Resources/Shaders/SkyBoxFragment.fs", GL_FRAGMENT_SHADER);
		GLuint skyVert = LoadShaderFromFile("Resources/Shaders/SkyBoxVertex.vs", GL_VERTEX_SHADER);

		GLuint SkyBoxShader = CreateProgram(skyVert, skyFrag);

		std::string skyboxPath = "Resources/Textures/skybox/";
		std::vector<std::string> faces = 
		{
			skyboxPath + "right.jpg",
			skyboxPath + "left.jpg",
			skyboxPath + "top.jpg",
			skyboxPath + "bottom.jpg",
			skyboxPath + "front.jpg",
			skyboxPath + "back.jpg"
		};

		unsigned int skyboxTexture;
		glGenTextures(1, &skyboxTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load( + faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		skybox = SkyBox(skyboxTexture, SkyBoxShader);
	}

	cameraUniformBufferHandler = new CameraUniformBufferHandler();
	directionalLightBufferHandler = new DirectionalLightBufferHandler();

	renderers = 
	{ 
		new SpriteRenderer(),
		new MeshRenderer()
	};
}

RenderingSystem::~RenderingSystem()
{
	delete(cameraUniformBufferHandler);
	delete(directionalLightBufferHandler);

	for (IRenderer* renderer : renderers)
	{
		delete(renderer);
	}
}

void RenderingSystem::DrawWorld()
{

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClearColor(.2, .2, .2, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	cameraUniformBufferHandler->BindCameraBuffers(world);
	directionalLightBufferHandler->BindDirectionalLightData(world);

	//skybox.Draw();

	for (IRenderer* renderer : renderers)
	{
		renderer->Draw(world);
	}

	//Draw framebuffer
	glUseProgram(PosProcessingProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0,0,0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(PosProcessingProgram, "depthTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, depthBuffertexture);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(PosProcessingProgram, "screenTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, buffertexture);
	quad.Draw();
}
