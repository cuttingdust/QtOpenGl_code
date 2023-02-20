#version 120

varying vec2 v_texcoord;

uniform sampler2D textureWall;
uniform sampler2D textureSmile;

void main()
{
   gl_FragColor = mix(texture2D(textureWall, v_texcoord), texture2D(textureSmile, v_texcoord), 0.5); 
}