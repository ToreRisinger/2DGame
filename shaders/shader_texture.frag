#version 330

out vec3 color;

in vec2 frag_text_coord;

uniform sampler2D texture_sampler;

void main()
{ 
	vec3 out_color = texture(texture_sampler, frag_text_coord).rgb;
	if (out_color.rgb == vec3(1.0,0.0,1.0))
      discard; 

	color = out_color;
}