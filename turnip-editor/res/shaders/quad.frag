#version 450 core
out vec4 out_color;

uniform sampler2D u_texture;
in vec2 v_uvs;

void main()
{
	out_color = vec4(v_uvs, 1.0, 0.0);
	out_color = texture(u_texture, v_uvs);
}