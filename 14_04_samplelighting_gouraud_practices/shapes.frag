#version 120

varying vec3 result;

void main()
{
   gl_FragColor = vec4(result, 1.0);
}