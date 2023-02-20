#version 120

attribute vec4 a_vertex;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;

void main()
{
  gl_Position = a_vertex;
  v_texcoord = a_texcoord;
}