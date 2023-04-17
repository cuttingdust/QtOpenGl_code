#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
  vec3 v_vertex;
  vec2 v_texcoord;
  vec3 v_normal;
} gs_in[];


out VS_OUT
{
  vec3 v_vertex;
  vec2 v_texcoord;
  vec3 v_normal;
} gs_out;

uniform float time;
uniform bool bExplode;

vec3 GetNormal() {
  vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
  return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
  float magnitude = 4.0;
  vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
  return position + vec4(direction, 0.0);
}



void main()
{
  gs_out.v_vertex = gs_in[0].v_vertex;
  gs_out.v_texcoord = gs_in[0].v_texcoord;
  gs_out.v_normal = gs_in[0].v_normal;

  vec3 normal = GetNormal();
  if(bExplode)
  {
    gl_Position = explode(gl_in[0].gl_Position, normal);
  }
  else
  {
    gl_Position = gl_in[0].gl_Position;
  }
  gs_out.v_texcoord = gs_in[0].v_texcoord;
  EmitVertex();

  if(bExplode)
  {
    gl_Position = explode(gl_in[1].gl_Position, normal);
  }
  else
  {
    gl_Position = gl_in[1].gl_Position;
  }
  gs_out.v_texcoord = gs_in[1].v_texcoord;
  EmitVertex();


  if(bExplode)
  {
    gl_Position = explode(gl_in[2].gl_Position, normal);
  }
  else
  {
    gl_Position = gl_in[2].gl_Position;
  }
  gs_out.v_texcoord = gs_in[2].v_texcoord;
  EmitVertex();


  EndPrimitive();
}