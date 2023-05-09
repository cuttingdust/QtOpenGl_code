#version 410 core

out vec3 v_vertex;
out vec2 v_texcoord;
out vec3 v_normal;

layout (location = 0) in vec3 a_vertex;
layout (location = 2) in vec3 a_normal;
layout (location = 1) in vec2 a_texcoord;

uniform mat4 model;
layout (std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

void main()
{
  v_vertex = vec3(model * vec4(a_vertex, 1.0));
  v_texcoord = a_texcoord;
  v_normal = mat3(transpose(inverse(model))) * a_normal;

  gl_Position = projection * view * model * vec4(a_vertex, 1.0);
}