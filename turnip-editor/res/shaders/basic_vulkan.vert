#version 450
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uvs;

layout(location = 0) out vec2 v_uvs;

void main()
{
	v_uvs = a_uvs;
	gl_Position = vec4(a_position, 1.0);
}