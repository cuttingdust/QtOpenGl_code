#version 120

varying vec4 vertexColor;

void main()
{
   gl_FragColor = vec4(vertexColor.xyz, 1.0);
}