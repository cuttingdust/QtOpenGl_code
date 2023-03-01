#version 120

varying vec3 v_normal;
varying vec3 v_vertex;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
   /// ambient
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor;

   // diffuse
   vec3 norm = normalize(v_normal);
   vec3 lightDir = normalize(lightPos - v_vertex);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;

   // specular
   float specularStrength = 0.5;
   vec3 viewDir = normalize(viewPos - v_vertex);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor;

   vec3 result = (ambient + diffuse + specular) * objectColor;
   gl_FragColor = vec4(result, 1.0);
}