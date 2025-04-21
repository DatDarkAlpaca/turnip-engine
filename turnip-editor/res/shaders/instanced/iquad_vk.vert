#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

layout (location = 0) out vec2 v_uvs;
layout (location = 1) flat out uint v_instance_texture_id;

layout (std140, binding = 0) uniform view_projection
{
    mat4 view;
    mat4 projection;
};

struct InstanceData
{
    float position[3];
    float scale[2];
    uint  textureLayer; 
};

layout (std430, binding = 1) readonly buffer ssbo_instance_data 
{
    InstanceData data[];
};

vec3 getInstancePosition(int index) 
{
    return vec3(data[index].position[0], data[index].position[1], data[index].position[2]);
}

vec2 getInstanceScale(int index) 
{
    return vec2(data[index].scale[0], data[index].scale[1]);
}

void main()
{
    vec2 instance_scale = getInstanceScale(gl_InstanceIndex);
    vec3 instance_pos = getInstancePosition(gl_InstanceIndex);

    vec3 world_pos = a_position * vec3(instance_scale, 1.0) + instance_pos;

    gl_Position = 
        projection 
        * view
        * vec4(world_pos, 1.0);

    v_uvs = a_uvs;
    v_instance_texture_id = data[gl_InstanceIndex].textureLayer;
}