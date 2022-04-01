#include "Texture2D.h"
#include <stdbool.h>
#include <stb_image.h>


Texture2D ImportTextureFromFile(const char* file, GLboolean alpha)
{
	GLuint format = GL_RGB;

	if (alpha)
		format = GL_RGBA;

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);
	
	Texture2D texture = { 0, width, height, format, format, GL_REPEAT, GL_REPEAT, GL_LINEAR};

	glGenTextures(1, &texture.ID);
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, texture.Internal_Format, width, height, 0, texture.Image_Format, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture.Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture.Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture.Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture.Filter_Max);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);
	return texture;
}
