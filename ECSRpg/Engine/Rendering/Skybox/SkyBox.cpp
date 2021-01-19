#include "SkyBox.h"
#include <glad/glad.h>

SkyBox::SkyBox(unsigned int Texture, unsigned int Shader)
{
	this->Texture = Texture;
	this->Shader = Shader;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void SkyBox::Draw()
{
	glDepthMask(GL_FALSE);
	glUseProgram(Shader);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}
