#include "game.h"

#include "game_object.h"
#include "debug_output.h"
#include "local_player.h"
#include "bot_player.h"
#include "character.h"
#include "character_soldier.h"
#include "graphics.h"
#include "gui_element_button.h"
#include "gui_element_hp_bar.h"
#include "gui_element_check_box.h"
#include "gui_element_skill_icons.h"
#include "callback.h"

#include <math.h>
#include <iostream>


int Game::WINDOW_WIDTH = 1280;
int Game::WINDOW_HEIGHT = 720;

Graphics* Game::graphics = nullptr;
Input* Game::input = nullptr;
Player* Game::local_player = nullptr;
std::vector<Player*> Game::players;
std::list<Game_Entity*> Game::entities;
GameLevel* Game::game_level = nullptr;
GameState Game::game_state = MAIN_MENU;

Game::Game()
{
	game_level = new GameLevel();
	game_level->set_texture_type(TextureType::GAME_LEVEL_TEXTURE_SAND);

	graphics = new Graphics("Game", WINDOW_WIDTH, WINDOW_HEIGHT, game_level, &entities, &players);

	input = new Input();

	local_player = new LocalPlayer(input);

	m_running = true;

	init_GUI();

	m_active_gui = m_main_menu_gui;
}

Game::~Game()
{

}

void Game::run()
{
	int fps;

	Timer sec;
	Timer update_timer;
	Timer last_update;

	sec.start();
	update_timer.start();
	last_update.start();

	fps = 0;

	while (m_running)
	{
		//-----INPUT-----
		this->handle_input(update_timer);



		//-----TICK-----
		this->on_tick(update_timer);




		//-----CLEAN-----
		this->remove_dead_entities();




		//-----RENDER-----
		graphics->clearScreen();

		this->draw_game();

		graphics->swapWindow();



		//-----FPS COUNTER-----
		fps++;
		if (sec.getTime() >= 1000)
		{
			sec.start();

			debug_output("Fps: " + std::to_string(fps));

			fps = 0;
		}
	}
}

void Game::draw_game()
{

	if (game_state == SINGLEPLAYER_GAME)
	{
		//SET CAMERA POSITION
		center_camera_on_local_player();

		//RENDER GAME_LEVEL
		game_level->draw();

		//RENDER ALL CHARACTERS
		for (Player* p : players)
		{
			p->getCharacter()->draw();
		}

		//RENDER ALL GAME ENTITIES
		for (Game_Entity* ge : entities)
		{
			ge->draw();
		}
		
	}
	else if (game_state == SINGLEPLAYER_GAME_PAUSE_MENU)
	{
		//SET CAMERA POSITION
		center_camera_on_local_player();	

		//RENDER GAME_LEVEL
		game_level->draw();

		//RENDER ALL CHARACTERS
		for (Player* p : players)
		{
			p->getCharacter()->draw();
		}

		//RENDER ALL GAME ENTITIES
		for (Game_Entity* ge : entities)
		{
			ge->draw();
		}
	}
	else
	{

	}

	m_active_gui->draw();
}


void Game::on_tick(Timer& last_update)
{
	
	if (last_update.getTime() > 0)
	{
		m_active_gui->on_tick(last_update.getTime());

		if (game_state == SINGLEPLAYER_GAME)
		{			

			//TICK ALL CHARACTERS
			for (Player* p : players)
			{
				p->on_tick(last_update.getTime());

				if (p->getCharacter()->is_dead())
				{
					p->getCharacter()->set_dead(false);
					p->getCharacter()->set_start_location(200, 200);
					p->getCharacter()->set_current_hp(100);
				}
			}

			//TICK ALL GAME ENTITIES
			for (Game_Entity* ge : entities)
			{
				ge->on_tick(last_update.getTime());
			}

			//TICK GAME_LEVEL
			game_level->on_tick(last_update.getTime());

			if (input->getKeyPressed(Input::KEYS::KEY_ESCAPE))
			{
				change_game_state(SINGLEPLAYER_GAME_PAUSE_MENU);
			}
		}
		else if (game_state == SINGLEPLAYER_GAME_PAUSE_MENU)
		{
			
		}

		last_update.start();
	}

}

void Game::remove_dead_entities()
{
	for (auto it = entities.begin(); it != entities.end();)
	{
		if ((*it)->is_dead())
		{
			delete *it;
			it = entities.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Game::handle_input(Timer& last_update)
{
	input->reset();

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		if (evnt.type == SDL_QUIT)
		{
			m_running = false;
		}
		else
		{
			input->handleInput(&evnt);			
		}
	}

	if (game_state == SINGLEPLAYER_GAME)
	{
		//Handle the input for all players
		for (Player* p : players)
		{
			p->handle_input(last_update.getTime());
		}
	}

	m_active_gui->handle_input();
}

void Game::change_game_state(GameState new_game_state)
{
	m_active_gui->reset();

	if (new_game_state == MAIN_MENU)
	{	
		m_active_gui = m_main_menu_gui;
	}
	else if (new_game_state == MULTIPLAYER_MENU)
	{
		m_active_gui = m_multiplayer_menu_gui;
	}
	else if (new_game_state == SINGLEPLAYER_MENU)
	{
		m_active_gui = m_singleplayer_menu_gui;
	}
	else if (new_game_state == LOADOUT_MENU)
	{
		m_active_gui = m_loadout_menu_gui;
	}
	else if (new_game_state == STARTING_NEW_SINGLEPLAYER_GAME)
	{
		//Start a new singleplayer game
		start_new_singleplayer_game();

		change_game_state(SINGLEPLAYER_GAME);
		return;
	}
	else if (new_game_state == SINGLEPLAYER_GAME)
	{
		m_active_gui = m_in_game_gui;
	}
	else if (new_game_state == SINGLEPLAYER_GAME_PAUSE_MENU)
	{
		m_active_gui = m_singleplayer_pause_menu_gui;
	}
	else if (EXIT_GAME)
	{
		m_running = false;
		game_state = new_game_state;
		return;
	}

	game_state = new_game_state;
}

void Game::start_new_singleplayer_game()
{
	m_game_level_option.dirt_type = TextureType::GAME_LEVEL_TEXTURE_DIRT;
	m_game_level_option.height = 400;
	m_game_level_option.width = 800;

	game_level->setup_new_game_level(m_game_level_option);

	graphics->init_game_level_graphics();

	//Reset player list
	while (players.size() > 0)
	{
		players.pop_back();
	}

	//-----LOCAL PLAYER-----

	//Give local player a character
	Character *c1 = new Character_Soldier();
	c1->set_owner(local_player);

	local_player->set_character(c1);

	//Set starting location
	local_player->getCharacter()->set_start_location(200, 200);

	//Remove the dirt around player
	game_level->destroyCircle(200, 200, 80);

	//Add local player to player list
	players.push_back(local_player);

	//-----BOT PLAYERS-----

	//Create new bot player
	BotPlayer* bot_player1 = new BotPlayer();

	//Give botplayer a character
	Character *c2 = new Character_Soldier();
	c2->set_owner(bot_player1);
	bot_player1->set_character(c2);

	//Set starting location
	bot_player1->getCharacter()->set_start_location(100, 100);

	//Remove the dirt around player
	game_level->destroyCircle(100, 100, 80);

	//Add bot to player list
	players.push_back(bot_player1);

	//Create new bot player
	BotPlayer* bot_player2 = new BotPlayer();

	//Give botplayer a character
	Character *c3 = new Character_Soldier();
	c3->set_owner(bot_player2);

	bot_player2->set_character(c3);

	//Set starting location
	bot_player2->getCharacter()->set_start_location(300, 300);

	//Remove the dirt around player
	game_level->destroyCircle(300, 300, 80);

	//Add bot to player list
	players.push_back(bot_player2);

	//center camera on local player
	center_camera_on_local_player();
}

void Game::center_camera_on_local_player()
{
	int x = local_player->getCharacter()->getX();
	int y = local_player->getCharacter()->getY();
	vec2 cam_pos = graphics->getCameraPosition();
	graphics->setCameraPosition(((-WINDOW_WIDTH / 2.0) + x), (-WINDOW_HEIGHT / 2.0) + y);
}


void Game::on_navigate_to_multiplayer_menu()
{
	change_game_state(MULTIPLAYER_MENU);	
}

void Game::on_navigate_to_singleplayer_menu()
{
	change_game_state(SINGLEPLAYER_MENU);
}

void Game::on_navigate_to_loadout_menu()
{
	change_game_state(LOADOUT_MENU);
}

void Game::on_navigate_to_main_menu()
{
	change_game_state(MAIN_MENU);	
}

void Game::on_navigate_to_exit_game()
{
	change_game_state(EXIT_GAME);
}

void Game::on_navigate_to_starting_new_singleplayer_Game()
{
	change_game_state(STARTING_NEW_SINGLEPLAYER_GAME);	
}

void Game::on_navigate_to_singleplayer_game()
{
	change_game_state(SINGLEPLAYER_GAME);	
}


void Game::on_navigate_to_singleplayer_game_pause_menu()
{
	change_game_state(SINGLEPLAYER_GAME_PAUSE_MENU);
}

void Game::init_GUI()
{
	//INIT MAIN MENU
	m_main_menu_gui = new Graphical_User_Interface();
	m_main_menu_gui->add_gui_element(new GUI_Element_Background(TextureType::GAME_LEVEL_TEXTURE_DIRT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_WIDTH));

	GUI_Element_Button* menu_button = new GUI_Element_Button("Multiplayer", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 200, 128, 64, 22, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_multiplayer_menu));
	m_main_menu_gui->add_gui_element(menu_button);

	menu_button = new GUI_Element_Button("Singleplayer", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 300, 128, 64, 20, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_singleplayer_menu));
	m_main_menu_gui->add_gui_element(menu_button);

	menu_button = new GUI_Element_Button("Loadout", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 400, 128, 64, 33, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_loadout_menu));
	m_main_menu_gui->add_gui_element(menu_button);

	menu_button = new GUI_Element_Button("Exit", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 600, 128, 64, 50, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_exit_game));
	m_main_menu_gui->add_gui_element(menu_button);

	//INIT MULTIPLAYER MENU
	m_multiplayer_menu_gui = new Graphical_User_Interface();
	m_multiplayer_menu_gui->add_gui_element(new GUI_Element_Background(TextureType::GAME_LEVEL_TEXTURE_DIRT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_WIDTH));

	menu_button = new GUI_Element_Button("Start", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 500, 128, 64, 46, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_multiplayer_menu));
	m_multiplayer_menu_gui->add_gui_element(menu_button);

	menu_button = new GUI_Element_Button("Back", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 600, 128, 64, 45, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_main_menu));
	m_multiplayer_menu_gui->add_gui_element(menu_button);

	//INIT SINGLEPLAYER MENU
	m_singleplayer_menu_gui = new Graphical_User_Interface();
	m_singleplayer_menu_gui->add_gui_element(new GUI_Element_Background(TextureType::GAME_LEVEL_TEXTURE_DIRT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_WIDTH));
	GUI_Element_Check_Box* check_box = new GUI_Element_Check_Box("Map borders", 150, Game::WINDOW_HEIGHT - 50, 32, 32, -100, 10);
	m_singleplayer_menu_gui->add_gui_element(check_box);

	menu_button = new GUI_Element_Button("Start", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 500, 128, 64, 46, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_starting_new_singleplayer_Game));
	m_singleplayer_menu_gui->add_gui_element(menu_button);

	menu_button = new GUI_Element_Button("Back", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 600, 128, 64, 45, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_main_menu));
	m_singleplayer_menu_gui->add_gui_element(menu_button);

	//INIT LOADOUT MENU
	m_loadout_menu_gui = new Graphical_User_Interface();
	m_loadout_menu_gui->add_gui_element(new GUI_Element_Background(TextureType::GAME_LEVEL_TEXTURE_DIRT, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_WIDTH));

	menu_button = new GUI_Element_Button("Back", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 600, 128, 64, 45, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_main_menu));
	m_loadout_menu_gui->add_gui_element(menu_button);

	//INIT SINGLEPLAYER PAUSE MENU
	m_singleplayer_pause_menu_gui = new Graphical_User_Interface();

	menu_button = new GUI_Element_Button("Resume", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 300, 128, 64, 37, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_singleplayer_game));
	m_singleplayer_pause_menu_gui->add_gui_element(menu_button);

	menu_button = new GUI_Element_Button("Quit", (WINDOW_WIDTH / 2), WINDOW_HEIGHT - 400, 128, 64, 45, 25);
	menu_button->set_callback(new Callback<Game>(this, &Game::on_navigate_to_main_menu));
	m_singleplayer_pause_menu_gui->add_gui_element(menu_button);

	//INIT INGAME
	m_in_game_gui = new Graphical_User_Interface();
	m_in_game_gui->add_gui_element(new GUI_Element_HP_Bar(local_player, 25, 50, 200, 30));
	m_in_game_gui->add_gui_element(new GUI_Element_Skill_Icons(local_player));
}