//#version 120

#version 410 core

uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
//    gl_FragColor = vec4(lightColor, 1.0);
    FragColor = vec4(lightColor, 1.0);
}