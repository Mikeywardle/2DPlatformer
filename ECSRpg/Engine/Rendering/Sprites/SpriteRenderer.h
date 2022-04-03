#pragma once

#include "../IRenderer.h"
#include <Maths/Vector2.h>
#include <Maths/Vector3.h>
#include <Maths/Color.h>
#include <glm/mat4x4.hpp>

#include <glad/glad.h>

struct SpriteVertex
{
	Vector3 position;
	Vector2 texCoords;
	Color color;
	float textureId;
};

struct Quad
{
	SpriteVertex topRight;
	SpriteVertex bottomRight;
	SpriteVertex bottomLeft;
	SpriteVertex topLeft;
};


class SpriteRenderer : public IRenderer
{
public:
	SpriteRenderer();

	virtual void Draw(World* world) override;

private:

	void GenerateIndices();
	Quad CreateSpriteVertices(Color color, float textureId, float batchIndex, SceneTransformComponent* transform, float Width, float Height, float PixelsPerUnit);
	void DrawData(int numberOfIndices, int boundTextures);

	void BindTextures(int boundTextures);


#define SPRITE_LIMIT 500

#define VERTEX_LIMIT SPRITE_LIMIT*4
#define INDEX_LIMIT SPRITE_LIMIT*6

	unsigned int indices[INDEX_LIMIT];
	Quad vertices[SPRITE_LIMIT];
	//glm::mat4 models[SPRITE_LIMIT];
	GLint textureSamplers[16];

	GLuint SpriteVAO, SpriteVBO, SpriteEBO;
	GLuint spriteShader;
};
