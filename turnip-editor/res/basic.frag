#version 410 core
out vec4 out_color;

uniform vec4 u_color = vec4(1.0);

in vec3 v_normals;
in vec4 v_color;
in vec2 v_uvs;

uniform sampler2D tex_slot_0;

void main()
{
	out_color = u_color * texture(tex_slot_0, v_uvs);
}