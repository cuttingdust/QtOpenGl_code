#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

uniform bool inverse_normals;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    vec3 n = inverse_normals ? -aNormal : aNormal;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.Normal = normalize(normalMatrix * n);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
