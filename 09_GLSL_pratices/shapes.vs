#version 120

attribute vec3 aPos;
attribute vec4 aColor;

varying vec4 ourColor;

uniform float xOffset;
void main()
{
  // gl_Position = vec4(aPos.x + xOffset, -aPos.y, aPos.z, 1.0);
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourColor = vec4(aPos, 1.0);
}