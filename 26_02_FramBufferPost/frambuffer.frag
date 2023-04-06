#version 410 core

in vec3 v_normal;
in vec3 v_vertex;
in vec2 v_texcoord;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct SPotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

uniform SPotLight spotLight;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform Material material;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool bSimpleZBudder;

vec4 diffuseTex = texture(material.diffuse, v_texcoord);
vec4 specularTex = texture(material.specular, v_texcoord);
vec3 CalcKenerl(sampler2D textureSample);

/// 1
//vec3 diffuseTexColor = vec3(diffuseTex);
//vec3 specularTexColor = vec3(specularTex);

/// 2
//vec3 diffuseTexColor = vec3(1.0 - diffuseTex);
//vec3 specularTexColor = vec3(1.0 - specularTex);

/// 3
//float diffAverge = (diffuseTex.r + diffuseTex.g + diffuseTex.b) / 3.0;
//vec3 diffuseTexColor = vec3(diffAverge, diffAverge, diffAverge);
//float speAverge = (specularTex.r + specularTex.g + specularTex.b) / 3.0;
//vec3 specularTexColor = vec3(speAverge, speAverge, speAverge);

/// 4
//float diffAverge = (diffuseTex.r * 0.2126 + diffuseTex.g * 0.7152 + diffuseTex.b * 0.0722) / 3.0;
//vec3 diffuseTexColor = vec3(diffAverge, diffAverge, diffAverge);
//float speAverge = (specularTex.r * 0.2126 + specularTex.g * 0.7152 + specularTex.b * 0.0722) / 3.0;
//vec3 specularTexColor = vec3(speAverge, speAverge, speAverge);

/// 5
vec3 diffuseTexColor;
vec3 specularTexColor;


float near = 0.1;
float far = 100.0;

const float offset = 1.0 / 300.0;

vec3 CalcSpotLight(SPotLight light, vec3 normal, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float LinearizeDepth(float depth);



void main()
{
    diffuseTexColor = CalcKenerl(material.diffuse);
    specularTexColor = CalcKenerl(material.specular);

    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(viewPos - v_vertex);

    vec3 result = vec3(0);
    result += CalcSpotLight(spotLight, norm, viewDir);
    result += CalcDirLight(dirLight, norm, viewDir);

    for (int i=0; i<NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, v_vertex, viewDir);

    if (!bSimpleZBudder)
    {
        FragColor = vec4(result, diffuseTex.a);
//        FragColor = vec4(CalcKenerl(material.diffuse), diffuseTex.a);

    }
    else
    {
        float depth = (LinearizeDepth(gl_FragCoord.z)-near) / (far-near); // divide by far for demonstration
        FragColor = vec4(vec3(depth), 1.0);
    }
}

vec3 CalcSpotLight(SPotLight light, vec3 normal, vec3 viewDir)
{
    // ambient
    vec3 ambient = diffuseTexColor * light.ambient;

    // diffuse
    vec3 lightDir = normalize(light.position - v_vertex);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseTexColor * light.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  spec*specularTexColor * light.specular;

    //attenuation
    float distance = length(light.position - v_vertex);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));
    //ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    //smooth
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // diffuse shading
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * diffuseTexColor;
    vec3 diffuse = light.diffuse * diff * diffuseTexColor;
    vec3 specular = light.specular * spec * specularTexColor;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // diffuse shading
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_texcoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_texcoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

float LinearizeDepth(float depth) {
    float z_ndc = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z_ndc  * (far - near));
}

vec3 CalcKenerl(sampler2D textureSample)
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2( 0.0f, offset), // top-center
        vec2( offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2( 0.0f, 0.0f), // center-center
        vec2( offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f, -offset), // bottom-center
        vec2( offset, -offset) // bottom-right
    );

//    float kernel[9] = float[](
//        1.0 / 16, 2.0 / 16, 1.0 / 16,
//        2.0 / 16, 4.0 / 16, 2.0 / 16,
//        1.0 / 16, 2.0 / 16, 1.0 / 16
//    );
    /// 4

    float kernel[9] = float[](
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(textureSample, v_texcoord.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    col += sampleTex[i] * kernel[i];

    return col;
}

