#version 450 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D depthTexture;
uniform sampler2D screenTexture;


const float offset = 1.0 / 200.0;  

vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);

vec3 LinearizeColor(vec3 depth) 
{
    const float near = 0.1; 
    const float far  = 10000.0; 

    float z = depth.r * 2.0 - 1.0; // back to NDC 
    float linearDepth =  (2.0 * near * far) / (far + near - z * (far - near));	
    return vec3(linearDepth,0,0);
}

vec3 ApplyKernalToTexture(float kernel[9])
{
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(depthTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += LinearizeColor(sampleTex[i]) * kernel[i];

    return col;
}

void main()
{ 
    float edgeKernel[9] = float[](
            1,1,1,
            1,-8,1,
            1,1,1
    );
    
    vec3 edgeCol = ApplyKernalToTexture(edgeKernel);

    vec3 col = vec3(1-step(edgeCol.r,.9));

    //FragColor = vec4(vec3(col),1);
    FragColor = mix(texture(screenTexture, TexCoords),vec4(vec3(0), 1.0), col.r);

}