#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
