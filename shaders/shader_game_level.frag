#version 330

out vec3 color;

in vec2 frag_text_coord;
in vec2 frag_mask_text_coord;
in vec2 frag_shadow_text_coord;

uniform sampler2D game_level_texture_sampler;
uniform sampler2D game_level_mask_texture_sampler;

void main()
{ 
	vec3 darken = vec3(1.0, 1.0, 1.0);
	if(texture(game_level_mask_texture_sampler, frag_mask_text_coord).r == 0)
	{
		darken = vec3(0.7, 0.7, 0.7);
		if(texture(game_level_mask_texture_sampler, frag_shadow_text_coord).r == 1)
		{
			darken = vec3(0.3, 0.3, 0.3);
		}
	}

	color = darken *  texture(game_level_texture_sampler, frag_text_coord).rgb;
   
}