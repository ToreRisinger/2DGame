#include "ingame_gui.h"

#include "item.h"

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
	Game_Object::m_graphics->render_texture_ui(TextureType::HP_BAR, 100 + bar_width / 2, 50, bar_width, 30, 0);

	std::vector<Item*>& items = m_character->get_items();

	int selected_item_index = m_character->get_selected_item_index();

	int half_width = 1280 / 2;

	int scaling = 15;

	int width = 60;

	for (int i = 0; i < items.size(); i++)
	{
		Game_Object::m_graphics->render_texture_ui(items.at(i)->get_item_icon_image(), half_width + 80 * i, 50, width + scaling * (int)(selected_item_index == i), width + scaling * (int)(selected_item_index == i), 0);
	}
	
}

void Ingame_GUI::set_character(Character* character)
{
	m_character = character;
}
