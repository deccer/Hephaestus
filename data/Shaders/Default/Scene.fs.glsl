#version 460 core

#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require // required for uint64_t type

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;

layout(location = 0) out vec4 o_color;

layout(location = 5) uniform uint u_material_index;

#include "GpuMaterial.include.glsl"

void main()
{
    GpuMaterial material = materialBuffer.Materials[u_material_index];
    vec4 color = material.BaseColorFactor.rgba;
    color *= texture(sampler2D(material.BaseColorTexture), v_uv).rgba;

    o_color = vec4(color.rgb, u_material_index);
}