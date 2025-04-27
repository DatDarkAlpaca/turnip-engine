#version 450 core
layout (location = 0) out vec4 out_color;
layout (location = 0) in vec2 v_uvs;

layout (binding  = 0) uniform sampler2D u_texture;

void main()
{
	out_color = texture(u_texture, v_uvs);
}