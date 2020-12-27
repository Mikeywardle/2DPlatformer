#version 450 core
out vec4 FragColor;  
 
in vec2 VTexCoord;
in vec4 VColor;
in float VTextureId;

uniform sampler2D F_Textures[16];

void main()
{
    int textureIndex = int(VTextureId);
    vec4 color = texture(F_Textures[textureIndex],VTexCoord) * VColor;

    if(color.a < 0.001)
        discard;

    FragColor = color;
} 