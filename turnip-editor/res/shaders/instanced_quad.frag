#version 450 core
layout (location = 0) out vec4 out_color;

layout (location = 0) in vec2 v_uvs;
layout (location = 1) flat in int v_instance_texture_id;

layout (binding = 0) uniform sampler2DArray u_texture_array;

void main()
{
    out_color = texture(u_texture_array, vec3(v_uvs, v_instance_texture_id));
}