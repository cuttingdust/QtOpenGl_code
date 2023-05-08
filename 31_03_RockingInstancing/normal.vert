#version 410 core
layout (location = 0) in vec3 a_vertex;
layout (location = 1) in vec2 a_texcoord;
layout (location = 2) in vec3 a_normal;

out VS_OUT { vec3 normal;} vs_out;

uniform mat4 model;
uniform mat4 view;


void main() { 
    vs_out.normal = mat3(transpose(inverse(view*model))) * a_normal;
//    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
//    vs_out.normal = normalize(vec3(vec4(normalMatrix * a_normal, 0.0)));

    gl_Position = view * model * vec4(a_vertex, 1.0);
} 
