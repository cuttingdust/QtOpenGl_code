#version 120

varying vec3 v_normal;
varying vec3 v_vertex;
varying vec2 v_texcoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
uniform Material material;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 diffuseTexColor = vec3(texture2D(material.diffuse, v_texcoord));
    vec3 specularTexColor = vec3(texture2D(material.specular, v_texcoord));

    vec3 lightDir = normalize(v_vertex - light.position);
    float theta = dot(lightDir, normalize(light.direction));

    if (theta > light.cutOff)
    {
        /// ambient
        vec3 ambient = diffuseTexColor * light.ambient;

        /// diffuse
        vec3 norm = normalize(v_normal);
        vec3 lightDir = normalize(light.position - v_vertex);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * diffuseTexColor * light.diffuse;

        /// specular
        vec3 viewDir = normalize(viewPos - v_vertex);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular =  spec * specularTexColor * light.specular;

        /// attenuation
        float distance = length(light.position - v_vertex);
        float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

        //ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);
        gl_FragColor = vec4(result, 1.0);
    }
    else
    {
        gl_FragColor = vec4(light.ambient * diffuseTexColor, 1.0);
    }
}
