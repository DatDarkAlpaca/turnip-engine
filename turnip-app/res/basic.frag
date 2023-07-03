#version 330 core
out vec4 out_color;

uniform vec4 u_color = vec4(1.0);
in vec4 v_color;

void main()
{
	out_color = v_color;
}