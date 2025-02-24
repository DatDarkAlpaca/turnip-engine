#version 450

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

layout(binding = 0) uniform mvp_object {
    mat4 model;
    mat4 view;
    mat4 projection;
} u_mvp;

layout (location = 0) out vec2 v_uvs;

void main() 
{
    v_uvs = a_uvs;
    gl_Position = u_mvp.projection * u_mvp.view * u_mvp.model * vec4(a_position, 1.0);
}