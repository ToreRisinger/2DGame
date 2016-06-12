#include "gui_element_background.h"

#include "game.h"

GUI_Element_Background::GUI_Element_Background(TextureType background_texture, int x, int y, int width, int height) 
	: GUI_Element{ x, y, width, height }, m_background_texture{background_texture}
{

}

GUI_Element_Background::~GUI_Element_Background()
{

}

void GUI_Element_Background::draw()
{
	Game::graphics->render_texture_ui(m_background_texture, m_x, m_y, m_width, m_height, 0);
}