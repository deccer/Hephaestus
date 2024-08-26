struct GpuMaterial {
    vec4 BaseColorFactor;
    uint64_t BaseColorTexture;
    uint64_t _padding1;
};

layout(binding = 2, std140) buffer MaterialBuffer
{
    GpuMaterial[] Materials;
} materialBuffer;