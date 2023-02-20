#version 120

varying vec2 v_texcoord;

uniform sampler2D textureWall;
uniform sampler2D textureSmile;
uniform sampler2D textureSmall;

void main()
{
   gl_FragColor = texture2D(textureSmall, v_texcoord);
}