#version 120

attribute vec3 a_vertex;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
  gl_Position = projection * view * model * vec4(a_vertex.x, a_vertex.y, a_vertex.z, 1.0);
  v_texcoord = a_texcoord;
}