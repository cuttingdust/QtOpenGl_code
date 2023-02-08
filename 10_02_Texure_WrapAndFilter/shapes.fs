#version 120

varying vec3 ourColor;
varying vec2 TexCord;

uniform sampler2D textureWall;
uniform sampler2D textureSmile;
uniform sampler2D textureSmall;

void main()
{
   gl_FragColor = texture2D(textureSmall, TexCord);
}