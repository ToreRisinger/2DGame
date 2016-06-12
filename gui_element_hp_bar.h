#pragma once

#include "gui_element.h"
#include "player.h"
#include "graphics.h"

class GUI_Element_HP_Bar : public GUI_Element
{
public:
	GUI_Element_HP_Bar(Player* player, int x, int y, int width, int height);
	~GUI_Element_HP_Bar();

	void draw();

private:
	Player* m_player;
	TextureType m_texture;
};

