#include "gui_element_skill_icons.h"

#include "player.h"
#include "character.h"
#include "skill.h"
#include "game.h"

GUI_Element_Skill_Icons::GUI_Element_Skill_Icons(Player* player) : GUI_Element{ 300, 200, 0, 0 }, m_player{player}
{

}

GUI_Element_Skill_Icons::~GUI_Element_Skill_Icons()
{

}

void GUI_Element_Skill_Icons::draw()
{
	
	std::vector<Skill*>* skills = m_player->getCharacter()->get_skills();

	int counter = 1;
	for (int i = 0; i < skills->size(); i++)
	{
		//HOTKEY
		if (i == 0) 
		{
			Game::graphics->render_dynamic_text("Q", Game::WINDOW_WIDTH / 2 - 360 + 140 * counter, 80, 0.4, glm::vec3{ 1.0, 1.0, 1.0 });
		}
		else if (i == 1) 
		{
			Game::graphics->render_dynamic_text("W", Game::WINDOW_WIDTH / 2 - 360 + 140 * counter, 80, 0.4, glm::vec3{ 1.0, 1.0, 1.0 });
		}
		else if (i == 2) 
		{
			Game::graphics->render_dynamic_text("E", Game::WINDOW_WIDTH / 2 - 360 + 140 * counter, 80, 0.4, glm::vec3{ 1.0, 1.0, 1.0 });
		}
		else if (i == 3) 
		{
			Game::graphics->render_dynamic_text("R", Game::WINDOW_WIDTH / 2 - 360 + 140 * counter, 80, 0.4, glm::vec3{ 1.0, 1.0, 1.0 });
		}
		
		//ICON
		Game::graphics->render_texture_ui(skills->at(i)->get_skill_icon_image(), Game::WINDOW_WIDTH / 2 - 340 + 140 * counter, 50, 120, 120, 0);

		counter++;
	}



}
