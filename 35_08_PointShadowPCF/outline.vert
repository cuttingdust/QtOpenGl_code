#version 410 core
layout (location = 0) in vec3 a_vertex;

uniform mat4 model;
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * model * vec4(a_vertex, 1.0);
}