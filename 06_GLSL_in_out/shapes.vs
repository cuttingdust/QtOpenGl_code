#version 120

attribute vec3 aPos
varying vec4 vertexColor;

void main()
{
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}