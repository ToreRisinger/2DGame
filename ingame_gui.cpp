#include "ingame_gui.h"

#include "item.h"
#include "game.h"

Ingame_GUI::Ingame_GUI()
{
	m_character = nullptr;
}

Ingame_GUI::~Ingame_GUI()
{

}

void Ingame_GUI::on_tick(unsigned int delta_time)
{

}

void Ingame_GUI::draw()
{
	int bar_width = m_character->get_current_hp();
	Game::graphics->render_texture_ui(TextureType::HP_BAR, 100 + bar_width / 2, 50, bar_width, 30, 0);

	int half_width = 1280 / 2;

	int scaling = 15;

	int width = 60;
	
}

void Ingame_GUI::set_character(Character* character)
{
	m_character = character;
}
