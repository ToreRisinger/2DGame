#include "gui_element_button.h"

#include "input.h"

GUI_Element_Button::GUI_Element_Button(std::string button_text, int x, int y, int width, int height, int text_offset_x, int text_offset_y) :
GUI_Element{ x, y, width, height }, 
m_button_idle{ TextureType::BUTTON_1_128X64_IDLE },
m_button_highlight{ TextureType::BUTTON_1_128X64_HIGHLIGHTED }
{
	m_active_texture = m_button_idle;

	m_left_x = x - width / 2;
	m_right_x = x + width / 2;

	m_top_y = y + height / 2;
	m_bottom_y = y - height / 2;

	m_button_text = button_text;

	m_callback_is_set = false;

	m_x_text_offset = text_offset_x;
	m_y_text_offset = text_offset_y;

	m_color = { 1.0, 1.0, 1.0 };
}

GUI_Element_Button::~GUI_Element_Button()
{

}

void GUI_Element_Button::draw()
{
	Game::graphics->render_texture_ui(m_active_texture, m_x, m_y, m_width, m_height, 0);
	Game::graphics->render_dynamic_text(m_button_text, m_left_x + m_x_text_offset, m_bottom_y + m_y_text_offset, 0.3, m_color);
}

void GUI_Element_Button::handle_input()
{
	if (mouse_hover_over_button(Game::input->getMouseX(), Game::WINDOW_HEIGHT - 1 - Game::input->getMouseY()))
	{
		m_active_texture = m_button_highlight;

		if (Game::input->getKeyPressed(Input::MOUSE_LEFT_BUTTON))
		{
			if (m_callback_is_set)
			{
				m_on_press->func();
			}
		}

	}
	else 
	{
		m_active_texture = m_button_idle;
	}

	
}

bool GUI_Element_Button::mouse_hover_over_button(int mouse_x, int mouse_y)
{
	if (mouse_x >= m_left_x && mouse_x <= m_right_x && mouse_y <= m_top_y && mouse_y >= m_bottom_y)
	{
		return true;
	}
	
	return false;
}

void GUI_Element_Button::set_callback(Callback<Game>* on_press)
{
	m_on_press = on_press;
	m_callback_is_set = true;
}

void GUI_Element_Button::reset()
{
	m_active_texture = m_button_idle;
}