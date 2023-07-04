#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normals;
layout (location = 2) in vec4 a_color;
layout (location = 3) in vec2 a_uvs;

uniform mat4 u_model = mat4(1.0);

out vec3 v_normals;
out vec4 v_color;
out vec2 v_uvs;

void main()
{
	gl_Position = u_model * vec4(a_position, 1.0);

	v_normals = a_normals;
	v_color = a_color;
	v_uvs = a_uvs;
}