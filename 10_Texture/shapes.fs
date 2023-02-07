#version 120

varying vec3 ourColor;
varying vec2 TexCord;

uniform sampler2D textureWall;

void main()
{
   gl_FragColor = texture2D(textureWall, TexCord);
}