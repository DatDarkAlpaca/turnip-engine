#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;
layout (location = 2) in mat4 a_instance_transform;
layout (location = 6) in int a_instance_texture_id; 

layout (location = 0)      out vec2 v_uvs;
layout (location = 1) flat out int v_instance_texture_id;

layout (std140, binding = 0) uniform view_projection
{
    mat4 view;
    mat4 projection;
};

void main()
{
    gl_Position = projection * view * a_instance_transform * vec4(a_position, 1.0);

    v_uvs = a_uvs;
    v_instance_texture_id = a_instance_texture_id;
}