#version 330

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_text_coord;
layout(location = 2) in vec2 in_mask_text_coord;
layout(location = 3) in vec2 in_shadow_text_coord;

out vec2 frag_text_coord;
out vec2 frag_mask_text_coord;
out vec2 frag_shadow_text_coord;

uniform mat4 translation;
uniform mat4 cam_translation;
uniform mat4 scale;
uniform mat4 projection;

void main()
{
	frag_text_coord = in_text_coord;
	frag_mask_text_coord = in_mask_text_coord;
	frag_shadow_text_coord = vec2(in_shadow_text_coord.x - 0.003, in_shadow_text_coord.y + 0.003);

	gl_Position = projection * cam_translation * translation * scale * vec4(in_pos, 1.0);
}