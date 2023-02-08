#version 120

varying vec3 ourColor;
varying vec2 TexCord;

uniform sampler2D textureWall;
uniform sampler2D textureSmile;

void main()
{
   gl_FragColor = mix(texture2D(textureWall, TexCord),texture2D(textureSmile, TexCord), 0.5);
}