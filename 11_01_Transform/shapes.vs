#version 120

attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTexCord;

varying vec3 ourColor;
varying vec2 TexCord;

uniform mat4 theMatrix;
void main()
{
  gl_Position = theMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  ourColor = aColor;
  TexCord = vec2(1 - aTexCord.s, aTexCord.t);
}