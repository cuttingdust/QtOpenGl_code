#version 120

attribute vec4 a_vertex;
attribute vec2 a_texcoord; //纹理坐标

varying vec2 v_texcoord; //2d纹理坐标


void main()
{
  gl_Position = a_vertex;
  v_texcoord = a_texcoord;
}