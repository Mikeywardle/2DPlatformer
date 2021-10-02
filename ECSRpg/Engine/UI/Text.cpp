#include "Text.h"
#include <glad/glad.h>
#include <Resources/Shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

FT_Library ft;
FT_Face face;

Character characters[128];

unsigned int textVAO, textVBO;

GLuint textShaderId;

void InitializeTextData()
{
	if (FT_Init_FreeType(&ft))
	{
		printf( "ERROR::FREETYPE: Could not init FreeType Library\n");
	}

	if (FT_New_Face(ft, "Resources/Fonts/arial.ttf", 0, &face))
	{
		printf("ERROR::FREETYPE: Failed to load font\n");
	}

	FT_Set_Pixel_Sizes(face, 0, 24);

    GLuint textVertex = LoadShaderFromFile("Resources/Shaders/TextVertex.vs", GL_VERTEX_SHADER);
    GLuint textFragment = LoadShaderFromFile("Resources/Shaders/TextFragment.fs",GL_FRAGMENT_SHADER);

    textShaderId = CreateProgram(textVertex, textFragment);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("ERROR::FREETYTPE: Failed to load Glyph\n");
		}

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            {(int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows},
            {(int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top},
            face->glyph->advance.x
        };

        characters[(int)c] = character;
	}
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void RenderText(std::string string, Vector2 position, Color color)
{
    //glUseProgram(textShaderId);
    //glUniform3f(glGetUniformLocation(textShaderId, "textColor"), color.r, color.g, color.b);
    //glActiveTexture(GL_TEXTURE0);
    //glBindVertexArray(textVAO);


    //glm::mat4 projection = glm::ortho(0.f, (float)WINDOW_WIDTH, 0.f, (float)WINDOW_HEIGHT, 0.f, 100.f);

    //glUniformMatrix4fv(glGetUniformLocation(textShaderId, "projection"), 1, GL_FALSE, &projection[0][0]);

    //for (int i = 0; i < string.length(); ++i)
    //{
    //    char c = string[i];
    //    Character ch = characters[(int)c];

    //    float xpos = position.x + ch.Bearing.x;
    //    float ypos = position.y - (ch.Size.y - ch.Bearing.y);

    //    float w = ch.Size.x;
    //    float h = ch.Size.y;

    //    float vertices[6][4] = {
    //         { xpos,     ypos + h,   0.0f, 0.0f },
    //         { xpos,     ypos,       0.0f, 1.0f },
    //         { xpos + w, ypos,       1.0f, 1.0f },

    //         { xpos,     ypos + h,   0.0f, 0.0f },
    //         { xpos + w, ypos,       1.0f, 1.0f },
    //         { xpos + w, ypos + h,   1.0f, 0.0f }
    //    };

    //    // render glyph texture over quad
    //    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    //    // update content of VBO memory
    //    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    //    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    // render quad
    //    glDrawArrays(GL_TRIANGLES, 0, 6);
    //    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    //    position.x += (ch.Advance >> 6);
    //}

    //glBindVertexArray(0);
    //glBindTexture(GL_TEXTURE_2D, 0);
}
