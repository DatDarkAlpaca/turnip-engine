#version 450 core
layout(location = 0) out vec3 a_position;
layout(location = 1) out vec3 a_colors;

out vec3 v_frag_color;

void main()
{
	gl_Position = vec4(a_position, 1.0);
	v_frag_color = a_colors;
}