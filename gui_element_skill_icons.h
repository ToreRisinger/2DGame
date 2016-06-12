#pragma once

#include "gui_element.h"
#include "graphics.h"

class Player;

class GUI_Element_Skill_Icons : public GUI_Element
{
public:
	GUI_Element_Skill_Icons(Player* player);
	~GUI_Element_Skill_Icons();

	void draw();

private:

	Player* m_player;

};

