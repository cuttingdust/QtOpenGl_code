#version 120

attribute vec3 a_vertex;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;

uniform mat4 theMatrix;
void main()
{
  gl_Position = theMatrix * vec4(a_vertex.x, a_vertex.y, a_vertex.z, 1.0);
  v_texcoord = vec2(1 - a_texcoord.s, a_texcoord.t);
}