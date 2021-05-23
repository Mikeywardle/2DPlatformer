#include "SpriteRenderer.h"

#include <stddef.h>
#include <stdlib.h>
#include <sstream>

#include <Camera/CameraComponent.h>
#include <Resources/Shader.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"


SpriteRenderer::SpriteRenderer()
{
	//Load and create Shaders
	GLuint spriteFrag = LoadShaderFromFile("Resources/Shaders/spriteFragmentShader.fs", GL_FRAGMENT_SHADER);
	GLuint spriteVert = LoadShaderFromFile("Resources/Shaders/spriteVertexShader.vs", GL_VERTEX_SHADER);

	spriteShader = CreateProgram(spriteVert, spriteFrag);

	// Generate Arrays
	glGenVertexArrays(1, &SpriteVAO);
	glGenBuffers(1, &SpriteVBO);
	glGenBuffers(1, &SpriteEBO);

	glBindVertexArray(SpriteVAO);

	glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, texCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, color));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)offsetof(SpriteVertex, textureId));


	//Generate Indicies array
	GenerateIndices();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void SpriteRenderer::Draw(World* world)
{
	glUseProgram(spriteShader);
	glBindVertexArray(SpriteVAO);

	int bufferedSprites = 0;

	std::unordered_map<int, int> textureLookup = std::unordered_map<int, int>();
	int boundTextures = 0;
	int textureIndex;

	std::vector<Entity> entities = world->GetEntities<SpriteComponent, Transform>();

	int batches = 0;
	for (Entity entity : entities)
	{
		SpriteComponent* s = world->GetComponent<SpriteComponent>(entity);
		Transform* t = world->GetComponent<Transform>(entity);

		const int SpriteTexture = s->GetTextureID();

		//If current texture not bound bind and add to lookup table
		if (textureLookup.count(SpriteTexture) == 0)
		{
			if (boundTextures == 16)
			{
				DrawData(bufferedSprites * 6, boundTextures);
				bufferedSprites = 0;
				boundTextures = 0;
				textureLookup.clear();
				++batches;
			}

			glActiveTexture(GL_TEXTURE0 + boundTextures);
			glBindTexture(GL_TEXTURE_2D, SpriteTexture);

			textureSamplers[boundTextures] = boundTextures;
			textureLookup[s->GetTextureID()] = boundTextures;
			textureIndex = boundTextures;
			++boundTextures;

		}
		else
		{
			textureIndex = textureLookup[SpriteTexture];
		}

		vertices[bufferedSprites] = CreateSpriteVertices(s->color, 
			textureIndex,
			bufferedSprites, 
			t,
			s->GetWidth(),
			s->GetHeight(),
			s->PixelsPerUnit);

		++bufferedSprites;
		if (bufferedSprites >= SPRITE_LIMIT)
		{
			DrawData(INDEX_LIMIT, boundTextures);
			bufferedSprites = 0;
			boundTextures = 0;
			textureLookup.clear();
			++batches;
		}
	}

	if (bufferedSprites > 0)
	{
		++batches;
		DrawData(bufferedSprites * 6, boundTextures);
	}

}

void SpriteRenderer::GenerateIndices()
{
	unsigned int indiciesPattern[6] = { 0,1,2,2,3,0 };
	int currentIndex;

	for (int i = 0; i < SPRITE_LIMIT; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			currentIndex = j + (i * 6);
			indices[currentIndex] = indiciesPattern[j] + (i * 4);
		}
	}
}

Quad SpriteRenderer::CreateSpriteVertices(Color color, float textureId, float batchIndex, Transform* transform, float Width, float Height, float PixelsPerUnit)
{
	float HalfQuadHeight = Height / (PixelsPerUnit * 2);
	float HalfQuadWidth = Width / (PixelsPerUnit * 2);

	Quad quad = {
		//Top Right
		transform->TransformPoint(Vector3(HalfQuadWidth,HalfQuadHeight,0)),
		Vector2(1.f, 1.f),
		color,
		textureId,

		//Bottom Right
		transform->TransformPoint(Vector3(HalfQuadWidth,-HalfQuadHeight,0)),
		Vector2(1.f, 0.f),
		color,
		textureId,

		//Bottom Left
		transform->TransformPoint(Vector3(-HalfQuadWidth,-HalfQuadHeight,0)),
		Vector2(0.f, 0.f),
		color,
		textureId,

		//Top Left
		transform->TransformPoint(Vector3(-HalfQuadWidth,HalfQuadHeight,0)),
		Vector2(0.f, 1.f),
		color,
		textureId,
	};

	return quad;
}

void SpriteRenderer::DrawData(int numberOfIndices, int boundTextures)
{
	glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	BindTextures(boundTextures);
	//glUniformMatrix4fv(glGetUniformLocation(spriteShader, "models"), 50, GL_FALSE, glm::value_ptr(models[0]));

	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::BindTextures(int boundTextures)
{
	std::string s;

	for (int i = 0; i < boundTextures; ++i)
	{
		std::ostringstream ss;
		ss << "F_Textures["<<i<<"]";
		glUniform1i(glGetUniformLocation(spriteShader,ss.str().c_str()), textureSamplers[i]);
	}



}
