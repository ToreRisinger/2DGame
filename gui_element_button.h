#pragma once

#include "gui_element.h"
#include "graphics.h"
#include "callback.h"
#include "game.h"

#include <glm/glm.hpp>

class GUI_Element_Button : public GUI_Element
{
public:
	GUI_Element_Button(std::string button_text, int x, int y, int width, int height, int text_x, int text_y);
	~GUI_Element_Button();

	void draw();
	void handle_input();

	void reset();

	void set_callback(Callback<Game>* on_press);

private:
	//Callback function
	Callback<Game>* m_on_press;

	bool m_callback_is_set;

	bool mouse_hover_over_button(int mouse_x, int mouse_y);

	TextureType m_active_texture;
	TextureType m_button_idle;
	TextureType m_button_highlight;

	int m_left_x;
	int m_right_x;

	int m_top_y;
	int m_bottom_y;

	int m_x_text_offset;
	int m_y_text_offset;

	std::string m_button_text;
	glm::vec3 m_color;
};

