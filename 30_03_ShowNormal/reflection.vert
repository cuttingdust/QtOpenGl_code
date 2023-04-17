#version 410 core
layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec3 a_normal;
out vec3 v_normal;
out vec3 v_vertex;
uniform mat4 model;
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};


void main() {
    v_normal = mat3(transpose(inverse(model))) * a_normal;
    v_vertex = vec3(model * vec4(a_vertex, 1.0));
    gl_Position = projection * view * vec4(v_vertex, 1.0);
}
