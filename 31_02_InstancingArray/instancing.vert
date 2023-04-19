#version 410 core

out VS_OUT
{
    vec3 v_vertex;
    vec2 v_texcoord;
    vec3 v_normal;
} vs_out;

layout (location = 0) in vec3 a_vertex;
layout (location = 2) in vec3 a_normal;
layout (location = 1) in vec2 a_texcoord;
layout (location = 3) in vec2 a_offset;

uniform mat4 model;
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform vec2 offsets[100];

void main()
{
//    vs_out.v_vertex = vec3(model * vec4(a_vertex, 1.0));
//    vs_out.v_texcoord = a_texcoord;
//    vs_out.v_normal = mat3(transpose(inverse(model))) * a_normal;
    vs_out.v_normal = a_normal;

//    vec2 offset = offsets[gl_InstanceID];
//    gl_Position = projection * view * model * vec4(a_vertex, 1.0);
    gl_Position = vec4(a_vertex, 1.0) + vec4(a_offset, 0.0, 0.0);
}