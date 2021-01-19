#version 450 core
out vec4 FragColor; 

 struct DirectionalLight 
 {
    float strength;
    vec3 direction;
    vec3 color;

 };

 layout (std140) uniform EyePositionBuffer
{
    vec3 eyePosition;
};

layout (std140) uniform DirectionalLightBuffer
{
    DirectionalLight light;
};
 
in vec2 VTexCoord;
in vec3 VNormal;
in vec3 FragPosition;

float near = 0.1; 
float far  = 10.0; 

uniform vec4 localColor;

float ambientStrength = 0.1f;
vec3 ambientColor = vec3(.3,.15,.4);


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
    vec3 localVec3 = vec3(localColor.x, localColor.y, localColor.z);
    vec3 ambient = ambientStrength * ambientColor*localVec3;

    vec3 norm = normalize(VNormal);
    vec3 lightDir = normalize(-light.direction);  

    //diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    float lightIntensity = smoothstep(0.0, 0.01, diff);
    vec3 diffuse = light.color * light.strength * localVec3 *lightIntensity;  

    //specular
    float specularStrength = .3f;
    vec3 viewDir = normalize(eyePosition - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm); 
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    float specularIntensitySmooth = smoothstep(0.005, 0.01, spec);
    vec3 specular = specularStrength * spec * light.color;  


    vec3 color = ambient + diffuse + specular;

    //Add fog
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration

    vec3 black = vec3(0,0,0);
//    color = mix(color, black, depth);

    FragColor = vec4(color, 1.0);
}