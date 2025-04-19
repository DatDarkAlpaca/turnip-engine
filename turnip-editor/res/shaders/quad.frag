#version 450 core
out vec4 out_color;

in vec2 v_uvs;
uniform sampler2D u_texture;

void main()
{
	out_color = texture(u_texture, v_uvs);
	//out_color = vec4(v_uvs, 1.0, 0.0);
}