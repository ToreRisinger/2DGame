#include "gui_element_hp_bar.h"

#include "game.h"

GUI_Element_HP_Bar::GUI_Element_HP_Bar(Player* player, int x, int y, int width, int height)
	: GUI_Element{ x, y, width, height }, m_texture{ TextureType::HP_BAR }, m_player{player}
{

}

GUI_Element_HP_Bar::~GUI_Element_HP_Bar()
{

}

void GUI_Element_HP_Bar::draw()
{
	float current_hp = m_player->getCharacter()->get_current_hp();
	float max_hp = m_player->getCharacter()->get_max_hp();

	float scale = current_hp / max_hp;
	int bar_width = m_width * scale;

	Game::graphics->render_dynamic_text(std::to_string((int)current_hp), m_x, m_y + 25, 1, glm::vec3{ 1.0, 1.0, 1.0 });
	Game::graphics->render_texture_ui(m_texture, m_x + bar_width / 2, m_y, bar_width, m_height, 0);
}
