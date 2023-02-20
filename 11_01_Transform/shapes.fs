#version 120

varying vec2 v_texcoord;

uniform sampler2D textureWall;
uniform sampler2D textureSmile;
uniform sampler2D textureSmall;

uniform float ratio;
void main()
{
   gl_FragColor = mix(texture2D(textureSmile, v_texcoord),
                     texture2D(textureWall, v_texcoord), ratio);
}