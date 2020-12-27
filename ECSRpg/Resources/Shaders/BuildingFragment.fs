#version 450 core
out vec4 FragColor; 

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float strength;
};

 
in vec2 VTexCoord;
in vec3 VNormal;
in vec3 FragPosition;

float near = 0.1; 
float far  = 100.0; 

vec3 localColor = vec3(6.f, .2f,0.f);

float ambientStrength = 0.1f;
vec3 ambientColor = vec3(.3,.15,.6);

DirectionalLight light =
{
    vec3(-1,-.8,.3),
    vec3(1,1,1),
    0.1
};


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //depth = 1.0-depth;

    vec3 ambient = ambientStrength * ambientColor*localColor;

    vec3 norm = normalize(VNormal);

    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * light.strength * localColor *diff;  

    vec3 color = ambient + diffuse ;

    FragColor = vec4(color, 1.0);
}