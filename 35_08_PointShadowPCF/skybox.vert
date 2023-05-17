#version 410 core
layout (location = 0) in vec3 a_vertex;
out vec3 v_texcoord;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform mat4 skyView;

void main() {
    v_texcoord = a_vertex;
    vec4 pos = projection * skyView * vec4(a_vertex, 1.0);
    gl_Position = pos.xyww;
}
