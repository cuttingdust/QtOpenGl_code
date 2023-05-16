#version 410 core
out vec4 FragColor;
in vec3 v_normal;
in vec3 v_vertex;
uniform vec3 viewPos;
uniform samplerCube skybox;
void main() {
    /// 类似钻石的效果
    float ratio = 1.00 / 2.42;
    vec3 I = normalize(v_vertex - viewPos);
    vec3 R = refract(I, normalize(v_normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
