#version 460 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec2 i_uv;
layout(location = 3) in vec4 i_tangent;

layout(location = 0) out vec3 v_position;
layout(location = 1) out vec3 v_normal;
layout(location = 2) out vec2 v_uv;

#include "GpuConstants.include.glsl"

layout(location = 0) uniform mat4 u_world_matrix;

void main()
{
    v_position = (u_world_matrix * vec4(i_position, 1.0)).xyz;
    v_normal = normalize(inverse(transpose(mat3(u_world_matrix))) * i_normal) + 0.00001 * vec3(i_tangent.xyz);
    v_uv = i_uv;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(v_position, 1.0);
}