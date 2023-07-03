#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 2) in vec4 a_color;

uniform mat4 u_model = mat4(1.0);

out vec4 v_color;

void main()
{
	gl_Position = u_model * vec4(a_position, 1.0);
	v_color = a_color;
}