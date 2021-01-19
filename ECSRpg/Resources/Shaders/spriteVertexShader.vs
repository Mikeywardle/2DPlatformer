#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color;
layout (location = 3) in float TextureId;
layout (location = 4) in float BatchIndex;

out vec2 VTexCoord;
out vec4 VColor;
out float VTextureId;

layout (std140) uniform CameraMatrices
{
    mat4 projection;
    mat4 view;
	mat4 transformlessView;
};

void main()
{
	VTexCoord = texCoord;
	VColor = color;
	VTextureId = TextureId;

	int modelIndex = int(BatchIndex);

	gl_Position = projection * view *vec4(position, 1.0);
}