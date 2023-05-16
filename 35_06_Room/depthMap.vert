#version 410 core
layout (location = 0) in vec3 a_vertex;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(a_vertex, 1.0);
}
