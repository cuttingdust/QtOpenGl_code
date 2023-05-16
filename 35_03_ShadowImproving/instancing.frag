#version 410 core

out vec4 FragColor;

in VS_OUT
{
    vec3 v_vertex;
    vec2 v_texcoord;
    vec3 v_normal;
} fs_in;

void main()
{
    FragColor = vec4(fs_in.v_normal, 1.0);
}

