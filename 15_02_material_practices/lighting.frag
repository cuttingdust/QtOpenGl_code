#version 120

uniform vec3 lightColor;

void main()
{
    gl_FragColor = vec4(lightColor, 1.0);
}