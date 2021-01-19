#version 450 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140) uniform CameraMatrices
{
    mat4 projection;
    mat4 view;
	mat4 transformlessView;
};

void main()
{
    TexCoords = aPos;
    gl_Position = projection * transformlessView * vec4(aPos, 1.0);
}  