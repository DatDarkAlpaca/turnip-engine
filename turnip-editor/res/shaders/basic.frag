#version 450 core
out vec4 out_color;

in vec2 v_uvs;

void main()
{
	out_color = vec4(v_uvs, 1.0, 1.0);
}