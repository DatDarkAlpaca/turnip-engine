#version 450 core
out vec4 outColor;

in vec3 v_frag_color;

void main()
{
	outColor = vec4(v_frag_color, 1.0);
}