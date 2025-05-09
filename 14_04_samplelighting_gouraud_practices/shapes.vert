#version 120

attribute vec3 a_vertex;
attribute vec3 a_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat3 inverse_mat3(mat3 m)
{
  float Determinant =
  m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
  - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
  + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);

  mat3 Inverse;
  Inverse[0][0] = + (m[1][1] * m[2][2] - m[2][1] * m[1][2]);
  Inverse[1][0] = - (m[1][0] * m[2][2] - m[2][0] * m[1][2]);
  Inverse[2][0] = + (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
  Inverse[0][1] = - (m[0][1] * m[2][2] - m[2][1] * m[0][2]);
  Inverse[1][1] = + (m[0][0] * m[2][2] - m[2][0] * m[0][2]);
  Inverse[2][1] = - (m[0][0] * m[2][1] - m[2][0] * m[0][1]);
  Inverse[0][2] = + (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
  Inverse[1][2] = - (m[0][0] * m[1][2] - m[1][0] * m[0][2]);
  Inverse[2][2] = + (m[0][0] * m[1][1] - m[1][0] * m[0][1]);
  Inverse /= Determinant;

  return Inverse;
}

mat4 inverse_mat4(mat4 m)
{
  float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
  float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
  float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

  float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
  float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
  float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

  float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
  float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
  float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

  float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
  float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
  float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

  float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
  float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
  float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

  float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
  float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
  float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

  const vec4 SignA = vec4( 1.0, -1.0,  1.0, -1.0);
  const vec4 SignB = vec4(-1.0,  1.0, -1.0,  1.0);

  vec4 Fac0 = vec4(Coef00, Coef00, Coef02, Coef03);
  vec4 Fac1 = vec4(Coef04, Coef04, Coef06, Coef07);
  vec4 Fac2 = vec4(Coef08, Coef08, Coef10, Coef11);
  vec4 Fac3 = vec4(Coef12, Coef12, Coef14, Coef15);
  vec4 Fac4 = vec4(Coef16, Coef16, Coef18, Coef19);
  vec4 Fac5 = vec4(Coef20, Coef20, Coef22, Coef23);

  vec4 Vec0 = vec4(m[1][0], m[0][0], m[0][0], m[0][0]);
  vec4 Vec1 = vec4(m[1][1], m[0][1], m[0][1], m[0][1]);
  vec4 Vec2 = vec4(m[1][2], m[0][2], m[0][2], m[0][2]);
  vec4 Vec3 = vec4(m[1][3], m[0][3], m[0][3], m[0][3]);

  vec4 Inv0 = SignA * (Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
  vec4 Inv1 = SignB * (Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
  vec4 Inv2 = SignA * (Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
  vec4 Inv3 = SignB * (Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

  mat4 Inverse = mat4(Inv0, Inv1, Inv2, Inv3);

  vec4 Row0 = vec4(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

  float Determinant = dot(m[0], Row0);

  Inverse /= Determinant;

  return Inverse;
}

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

varying vec3 result;

void main()
{
  vec3 normal = mat3(transpose(inverse_mat4(model))) * a_normal;
  vec3 vertex = vec3(model * vec4(a_vertex, 1.0));
  gl_Position = projection * view * model * vec4(a_vertex, 1.0);

  /// ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - vertex);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - vertex);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  result = (ambient + diffuse + specular) * objectColor;
}