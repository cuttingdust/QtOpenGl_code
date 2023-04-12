#version 410 core
layout (location = 0) in vec3 a_vertex;
out vec3 v_texcoord;
uniform mat4 projection;
uniform mat4 view;
void main() {
    v_texcoord = a_vertex;
    vec4 pos = projection * view * vec4(a_vertex, 1.0);
    gl_Position = pos.xyww;
}
