struct GpuModelMeshInstance {
    mat4 WorldMatrix;
};

layout(binding = 1, std430) readonly buffer GpuModelMeshInstanceBuffer {
    GpuModelMeshInstance Instances[];
} modelMeshInstanceBuffer;