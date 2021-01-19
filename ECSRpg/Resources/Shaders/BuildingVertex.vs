#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;


out vec2 VTexCoord;
out vec3 VNormal;
out vec3 FragPosition;

uniform mat4 model;

layout (std140) uniform CameraMatrices
{
    mat4 projection;
    mat4 view;
	mat4 transformlessView;
};

void main()
{
	VTexCoord = texCoord;
	VNormal = mat3(transpose(inverse(model))) * normal;
	gl_Position = projection * view  * model * vec4(position, 1.0);
	FragPosition = vec3(model * vec4(position, 1.0));
}