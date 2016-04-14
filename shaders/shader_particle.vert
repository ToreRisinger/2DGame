#version 330

layout(location = 0) in vec3 in_vertex_pos;
layout(location = 1) in vec3 in_position;

out vec3 frag_color;

uniform mat4 cam_translation;
uniform mat4 scale;
uniform vec3 color;
uniform mat4 projection;

void main()
{
	frag_color = color;

	mat4 translation = mat4(1.0);
	translation[3][0] = in_position.x;
	translation[3][1] = in_position.y;
	translation[3][2] =	0;

	gl_Position =  projection * cam_translation * translation * scale * vec4(in_vertex_pos, 1.0);
}