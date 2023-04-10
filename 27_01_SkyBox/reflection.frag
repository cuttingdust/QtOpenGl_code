#version 410 core
out vec4 FragColor;
in vec3 v_normal;
in vec3 v_vertex;
uniform vec3 viewPos;
uniform samplerCube skybox;
void main() {
    vec3 I = normalize(v_vertex - viewPos);
    vec3 R = reflect(I, normalize(v_normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
