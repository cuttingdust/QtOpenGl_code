#version 120

varying vec2 TexCord;

uniform sampler2D textureWall;
uniform sampler2D textureSmile;
uniform sampler2D textureSmall;

uniform float ratio;
void main()
{
   gl_FragColor = mix(texture2D(textureSmile, TexCord),
                     texture2D(textureWall, TexCord), ratio);
}