#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;
in VS_OUT {
    vec3 v_color;
    vec3 v_vertex;
    vec2 v_texcoord;
    vec3 v_normal;
} gs_in[];


out vec3 fColor;

void build_house(vec4 position) {
    fColor = gs_in[0].v_color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2, 0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0, 0.4, 0.0, 0.0); // 5:top
    fColor=vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}
void main() {
    build_house(gl_in[0].gl_Position);
}
