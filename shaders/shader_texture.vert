#version 330

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_text_coord;

out vec2 frag_text_coord;

uniform mat4 translation;
uniform mat4 cam_translation;
uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 projection;

void main()
{
	frag_text_coord = in_text_coord;

	gl_Position =   projection * cam_translation * translation * scale * rotation * vec4(in_pos, 1.0);
}