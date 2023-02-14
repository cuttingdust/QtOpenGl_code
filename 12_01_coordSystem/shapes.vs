#version 120

attribute vec3 aPos;
attribute vec2 aTexCord;

varying vec2 TexCord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
  gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  TexCord = aTexCord;
}