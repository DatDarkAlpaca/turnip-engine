#version 450 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

layout (location = 0) out vec2 v_uvs;

layout(binding = 0, std140) uniform u_mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

void main()
{
	v_uvs = a_uvs;
	gl_Position = projection * view * model * vec4(a_position, 1.0);
}