/// shapes.frag
//#version 120

//varying vec3 v_normal;
//varying vec3 v_vertex;
//varying vec2 v_texcoord;

#version 410 core

in VS_OUT
{
    vec3 v_normal;
    vec3 v_vertex;
    vec2 v_texcoord;

}fs_in;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_emission1;
    sampler2D texture_reflection1;
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
uniform bool bReflectTexturteSkyBox;

//vec3 diffuseTexColor = vec3(texture2D(material.diffuse, v_texcoord));
//vec3 specularTexColor = vec3(texture2D(material.specular, v_texcoord));

vec3 diffuseTexColor = vec3(texture(material.texture_diffuse1, fs_in.v_texcoord));
vec3 specularTexColor = vec3(texture(material.texture_specular1, fs_in.v_texcoord));
vec3 reflectionTexColor = vec3(texture(material.texture_reflection1, fs_in.v_texcoord));
float near = 0.1;
float far = 100.0;

vec3 CalcSpotLight(SPotLight light, vec3 normal, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float LinearizeDepth(float depth);

uniform samplerCube skybox;
uniform bool blinn;
uniform bool bPointLight;
uniform bool bSPointLight;
uniform bool bDirLight;
void main()
{
    vec3 norm = normalize(fs_in.v_normal);
    vec3 viewDir = normalize(viewPos - fs_in.v_vertex);
    vec3 reflectDir = reflect(-viewDir, norm);

    vec3 result = vec3(0);
    if (bSPointLight)
    {
        result += CalcSpotLight(spotLight, norm, viewDir);
    }

    if (bDirLight)
    {
        result += CalcDirLight(dirLight, norm, viewDir);
    }


    if (bPointLight)
    {
        for (int i=0; i<NR_POINT_LIGHTS; i++)
            result += CalcPointLight(pointLights[i], norm, fs_in.v_vertex, viewDir);
    }


    /// reflection
    vec3 reflectionSkyBox = texture(skybox, reflectDir).rgb;
    vec3 reflection= reflectionTexColor * reflectionSkyBox;
    if (bReflectTexturteSkyBox)
    {
        /// 可以用于处理 渲染左右两个不一样的渲染效果
        if (gl_FragCoord.x > 320)
            result += reflection;
    }


    if (!bSimpleZBudder)
    {
        FragColor = vec4(result, 1.0);
        FragColor = vec4(result, 1.0);
    }
    else
    {
        float depth = (LinearizeDepth(gl_FragCoord.z)-near) / (far-near); // divide by far for demonstration
        FragColor = vec4(vec3(depth), 1.0);
    }


}

vec3 CalcSpotLight(SPotLight light, vec3 normal, vec3 viewDir)
{
    /// ambient
    vec3 ambient = diffuseTexColor * light.ambient;

    /// diffuse
    vec3 lightDir = normalize(light.position - fs_in.v_vertex);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseTexColor * light.diffuse;

    /// specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  spec*specularTexColor * light.specular;

    //attenuation
    float distance = length(light.position - fs_in.v_vertex);
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

   if(blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }


    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.v_texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.v_texcoord));
    vec3 specular = light.specular * spec * 0.6;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

float LinearizeDepth(float depth) {
    float z_ndc = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z_ndc  * (far - near));
}

