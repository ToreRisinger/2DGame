#include "game.h"

#include "game_object.h"
#include "debug_output.h"
#include "local_player.h"
#include "bot_player.h"
#include "character.h"
#include "graphics.h"

#include <math.h>
#include <iostream>

Game::Game()
{
	m_debug_mode = true;

	m_window_width = 1280;
	m_window_height = 720;

	m_game_level = new GameLevel(800, 400);
	m_game_level->set_texture_type(TextureType::GAME_LEVEL_TEXTURE_SAND);

	m_graphics = new Graphics("Game", m_window_width, m_window_height, m_game_level, &m_entities, &m_players);

	m_input = new Input();

	m_local_player = new LocalPlayer(m_input);

	m_ingame_GUI = new Ingame_GUI();

	Game_Object::set_game_level(m_game_level);
	Game_Object::set_players(&m_players);
	Game_Object::set_game_entities(&m_entities);
	Game_Object::set_graphics(m_graphics);

	m_running = true;

	initGUI();

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
		m_graphics->clearScreen();

		this->draw_game();

		m_graphics->swapWindow();



		//-----FPS COUNTER-----
		fps++;
		if (sec.getTime() >= 1000)
		{
			sec.start();

			if (m_debug_mode)
			{
				debug_output("Fps: " + std::to_string(fps));
			}

			fps = 0;
		}
	}
}

void Game::draw_game()
{
	if (m_game_state == SINGLEPLAYER_GAME)
	{
		//SET CAMERA POSITION
		center_camera_on_local_player();

		//RENDER GAME_LEVEL
		m_game_level->draw();

		//RENDER ALL CHARACTERS
		for (Player* p : m_players)
		{
			p->getCharacter()->draw();
		}

		//RENDER ALL GAME ENTITIES
		for (Game_Entity* ge : m_entities)
		{
			ge->draw();
		}

		//RENDER INGAME GUI
		m_ingame_GUI->draw();
	}
	else if (m_game_state == SINGLEPLAYER_GAME_PAUSE_MENU)
	{
		//SET CAMERA POSITION
		center_camera_on_local_player();	

		//RENDER ALL CHARACTERS
		for (Player* p : m_players)
		{
			p->getCharacter()->draw();
		}

		//RENDER ALL GAME ENTITIES
		for (Game_Entity* ge : m_entities)
		{
			ge->draw();
		}

		//RENDER GAME_LEVEL
		m_game_level->draw();

		//RENDER CURSOR AND GUI
		m_active_gui->draw();
		m_cursor_gui->draw();
	}
	else
	{
		//RENDER CURSOR AND GUI
		m_active_gui->draw();
		m_cursor_gui->draw();
	}
}


void Game::on_tick(Timer& last_update)
{
	//Update time on GUI
	m_cursor_gui->update();
	m_active_gui->update();

	if (m_game_state == SINGLEPLAYER_GAME)
	{
		if (last_update.getTime() > 0) 
		{
			int time_passed = last_update.getTime();
			last_update.start();
			
			//TICK ALL CHARACTERS
			for (Player* p : m_players)
			{
				p->on_tick(time_passed);

				if (p->getCharacter()->is_dead()) 
				{
					p->getCharacter()->set_dead(false);
					p->getCharacter()->set_start_location(200, 200);
					p->getCharacter()->set_current_hp(100);
				}
			}

			//TICK ALL GAME ENTITIES
			for (Game_Entity* ge : m_entities)
			{
				ge->on_tick(time_passed);
			}

			//TICK GAME_LEVEL
			m_game_level->on_tick(time_passed);

			//TICK INGAME GUI
			m_ingame_GUI->on_tick(time_passed);
		}
	
		if (m_input->getKeyPressed(Input::KEYS::KEY_ESCAPE))
		{
			change_game_state(SINGLEPLAYER_GAME_PAUSE_MENU);
		}
	}
	else if (m_game_state == SINGLEPLAYER_GAME_PAUSE_MENU)
	{
		if (last_update.getTime() > 0) {
			last_update.start();
		}
	}

}

void Game::remove_dead_entities()
{
	for (auto it = m_entities.begin(); it != m_entities.end();)
	{
		if ((*it)->is_dead())
		{
			delete *it;
			it = m_entities.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Game::handle_input(Timer& last_update)
{
	m_input->reset();

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		if (evnt.type == SDL_QUIT)
		{
			m_running = false;
		}
		else
		{
			m_input->handleInput(&evnt);

			m_active_gui->handle_input(evnt);
			m_cursor_gui->handle_input(evnt);
		}
	}

	if (m_game_state == SINGLEPLAYER_GAME)
	{
		//Handle the input for all players
		for (Player* p : m_players)
		{
			p->handle_input(last_update.getTime());
		}
	}
}

void Game::change_game_state(GameState new_game_state)
{
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
		m_active_gui = m_singleplayer_game_gui;
	}
	else if (new_game_state == SINGLEPLAYER_GAME_PAUSE_MENU)
	{
		m_active_gui = m_singleplayer_game_pause_menu_gui;
	}
	else if (EXIT_GAME)
	{
		m_running = false;
		m_game_state = new_game_state;
		return;
	}

	m_active_gui->mouse_motion(m_input->getMouseX(), m_input->getMouseY());

	m_game_state = new_game_state;
}

void Game::start_new_singleplayer_game()
{
	//Reset player list
	while (m_players.size() > 0)
	{
		m_players.pop_back();
	}

	//-----LOCAL PLAYER-----
	//Give local player a character
	m_local_player->set_character(new Character(m_local_player));
	m_ingame_GUI->set_character(m_local_player->getCharacter());

	//Set starting location
	m_local_player->getCharacter()->set_start_location(200, 200);

	//Remove the dirt around player
	m_game_level->destroyCircle(200, 200, 80);

	//Add local player to player list
	m_players.push_back(m_local_player);

	//-----BOT PLAYERS-----

	//Create new bot player
	BotPlayer* bot_player1 = new BotPlayer();

	//Give botplayer a character
	bot_player1->set_character(new Character(bot_player1));

	//Set starting location
	bot_player1->getCharacter()->set_start_location(100, 100);

	//Remove the dirt around player
	m_game_level->destroyCircle(100, 100, 80);

	//Add bot to player list
	m_players.push_back(bot_player1);

	//Create new bot player
	BotPlayer* bot_player2 = new BotPlayer();

	//Give botplayer a character
	bot_player2->set_character(new Character(bot_player2));

	//Set starting location
	bot_player2->getCharacter()->set_start_location(300, 300);

	//Remove the dirt around player
	m_game_level->destroyCircle(300, 300, 80);

	//Add bot to player list
	m_players.push_back(bot_player2);

	//center camera on local player
	center_camera_on_local_player();
}

void Game::center_camera_on_local_player()
{
	int x = m_local_player->getCharacter()->getX();
	int y = m_local_player->getCharacter()->getY();
	vec2 cam_pos = m_graphics->getCameraPosition();
	m_graphics->setCameraPosition(((-m_window_width / 2.0) + x), (-m_window_height / 2.0) + y);
}


bool Game::on_navigate_to_multiplayer_menu(const CEGUI::EventArgs& e)
{
	change_game_state(MULTIPLAYER_MENU);
	return true;
}

bool Game::on_navigate_to_singleplayer_menu(const CEGUI::EventArgs& e)
{
	change_game_state(SINGLEPLAYER_MENU);
	return true;
}

bool Game::on_navigate_to_loadout_menu(const CEGUI::EventArgs& e)
{
	change_game_state(LOADOUT_MENU);
	return true;
}

bool Game::on_navigate_to_main_menu(const CEGUI::EventArgs& e)
{
	change_game_state(MAIN_MENU);
	return true;
}

bool Game::on_navigate_to_exit_game(const CEGUI::EventArgs& e)
{
	change_game_state(EXIT_GAME);
	return true;
}

bool Game::on_navigate_to_starting_new_singleplayer_Game(const CEGUI::EventArgs& e)
{
	change_game_state(STARTING_NEW_SINGLEPLAYER_GAME);
	return true;
}

bool Game::on_navigate_to_singleplayer_game(const CEGUI::EventArgs& e)
{
	change_game_state(SINGLEPLAYER_GAME);
	return true;
}


bool Game::on_navigate_to_singleplayer_game_pause_menu(const CEGUI::EventArgs& e)
{
	change_game_state(SINGLEPLAYER_GAME_PAUSE_MENU);
	return true;
}

void Game::initGUI() 
{
	//Init cursor gui
	m_cursor_gui = new GUI();
	m_cursor_gui->init("GUI");
	m_cursor_gui->loadScheme("AlfiskoSkin.scheme");
	m_cursor_gui->setFont("DejaVuSans-10");

	m_cursor_gui->setMouseCursor("AlfiskoSkin/MouseArrow");
	m_cursor_gui->showMouseCursor();

	//Init main menu gui
	m_main_menu_gui = new GUI();

	m_main_menu_gui->init("GUI");
	m_main_menu_gui->loadScheme("AlfiskoSkin.scheme");
	m_main_menu_gui->setFont("DejaVuSans-10");

	CEGUI::PushButton* main_menu_button_1 = static_cast<CEGUI::PushButton*>(m_main_menu_gui->createWidget("AlfiskoSkin/Button", "main_menu_button_1"));
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_1, 0, 0);
	m_main_menu_gui->setWidgetSizePerc(main_menu_button_1, 0.1, 0.05);
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_1, 0.5 - main_menu_button_1->getWidth().d_scale / 2.0, 0.4 - main_menu_button_1->getHeight().d_scale / 2.0);
	main_menu_button_1->setText("Multiplayer");
	main_menu_button_1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_multiplayer_menu, this));

	CEGUI::PushButton* main_menu_button_2 = static_cast<CEGUI::PushButton*>(m_main_menu_gui->createWidget("AlfiskoSkin/Button", "main_menu_button_2"));
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_2, 0, 0);
	m_main_menu_gui->setWidgetSizePerc(main_menu_button_2, 0.1, 0.05);
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_2, 0.5 - main_menu_button_2->getWidth().d_scale / 2.0, 0.5 - main_menu_button_2->getHeight().d_scale / 2.0);
	main_menu_button_2->setText("Singleplayer");
	main_menu_button_2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_singleplayer_menu, this));

	CEGUI::PushButton* main_menu_button_3 = static_cast<CEGUI::PushButton*>(m_main_menu_gui->createWidget("AlfiskoSkin/Button", "main_menu_button_3"));
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_3, 0, 0);
	m_main_menu_gui->setWidgetSizePerc(main_menu_button_3, 0.1, 0.05);
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_3, 0.5 - main_menu_button_3->getWidth().d_scale / 2.0, 0.6 - main_menu_button_3->getHeight().d_scale / 2.0);
	main_menu_button_3->setText("Loadout");
	main_menu_button_3->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_loadout_menu, this));

	CEGUI::PushButton* main_menu_button_4 = static_cast<CEGUI::PushButton*>(m_main_menu_gui->createWidget("AlfiskoSkin/Button", "main_menu_button_4"));
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_4, 0, 0);
	m_main_menu_gui->setWidgetSizePerc(main_menu_button_4, 0.1, 0.05);
	m_main_menu_gui->setWidgetPositionPerc(main_menu_button_4, 0.5 - main_menu_button_4->getWidth().d_scale / 2.0, 0.75);
	main_menu_button_4->setText("Exit");
	main_menu_button_4->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_exit_game, this));

	//Init multiplayer menu
	m_multiplayer_menu_gui = new GUI();

	m_multiplayer_menu_gui->init("GUI");
	m_multiplayer_menu_gui->loadScheme("AlfiskoSkin.scheme");
	m_multiplayer_menu_gui->setFont("DejaVuSans-10");

	CEGUI::PushButton* multiplayer_menu_button_1 = static_cast<CEGUI::PushButton*>(m_multiplayer_menu_gui->createWidget("AlfiskoSkin/Button", "multiplayer_menu_button_1"));
	m_multiplayer_menu_gui->setWidgetPositionPerc(multiplayer_menu_button_1, 0, 0);
	m_multiplayer_menu_gui->setWidgetSizePerc(multiplayer_menu_button_1, 0.1, 0.05);
	m_multiplayer_menu_gui->setWidgetPositionPerc(multiplayer_menu_button_1, 0.5 - multiplayer_menu_button_1->getWidth().d_scale / 2.0, 0.75);
	multiplayer_menu_button_1->setText("Back");
	multiplayer_menu_button_1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_main_menu, this));

	//Init singleplayer menu
	m_singleplayer_menu_gui = new GUI();

	m_singleplayer_menu_gui->init("GUI");
	m_singleplayer_menu_gui->loadScheme("AlfiskoSkin.scheme");
	m_singleplayer_menu_gui->setFont("DejaVuSans-10");


	CEGUI::PushButton* singleplayer_menu_button_1 = static_cast<CEGUI::PushButton*>(m_singleplayer_menu_gui->createWidget("AlfiskoSkin/Button", "singleplayer_menu_button_1"));
	m_singleplayer_menu_gui->setWidgetPositionPerc(singleplayer_menu_button_1, 0, 0);
	m_singleplayer_menu_gui->setWidgetSizePerc(singleplayer_menu_button_1, 0.1, 0.05);
	m_singleplayer_menu_gui->setWidgetPositionPerc(singleplayer_menu_button_1, 0.5 - singleplayer_menu_button_1->getWidth().d_scale / 2.0, 0.65);
	singleplayer_menu_button_1->setText("Start game");
	singleplayer_menu_button_1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_starting_new_singleplayer_Game, this));

	CEGUI::PushButton* singleplayer_menu_button_2 = static_cast<CEGUI::PushButton*>(m_singleplayer_menu_gui->createWidget("AlfiskoSkin/Button", "singleplayer_menu_button_2"));
	m_singleplayer_menu_gui->setWidgetPositionPerc(singleplayer_menu_button_2, 0, 0);
	m_singleplayer_menu_gui->setWidgetSizePerc(singleplayer_menu_button_2, 0.1, 0.05);
	m_singleplayer_menu_gui->setWidgetPositionPerc(singleplayer_menu_button_2, 0.5 - singleplayer_menu_button_2->getWidth().d_scale / 2.0, 0.75);
	singleplayer_menu_button_2->setText("Back");
	singleplayer_menu_button_2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_main_menu, this));

	//Init loadout menu
	m_loadout_menu_gui = new GUI();

	m_loadout_menu_gui->init("GUI");
	m_loadout_menu_gui->loadScheme("AlfiskoSkin.scheme");
	m_loadout_menu_gui->setFont("DejaVuSans-10");

	CEGUI::PushButton* loadout_menu_button_1 = static_cast<CEGUI::PushButton*>(m_loadout_menu_gui->createWidget("AlfiskoSkin/Button", "loadout_menu_button_1"));
	m_loadout_menu_gui->setWidgetPositionPerc(loadout_menu_button_1, 0, 0);
	m_loadout_menu_gui->setWidgetSizePerc(loadout_menu_button_1, 0.1, 0.05);
	m_loadout_menu_gui->setWidgetPositionPerc(loadout_menu_button_1, 0.5 - loadout_menu_button_1->getWidth().d_scale / 2.0, 0.75);
	loadout_menu_button_1->setText("Back");
	loadout_menu_button_1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_main_menu, this));

	//Init singleplayer ingame gui
	m_singleplayer_game_gui = new GUI();

	m_singleplayer_game_gui->init("GUI");
	m_singleplayer_game_gui->loadScheme("AlfiskoSkin.scheme");
	m_singleplayer_game_gui->setFont("DejaVuSans-10");


	//Init singleplayer ingame pause menu
	m_singleplayer_game_pause_menu_gui = new GUI();

	m_singleplayer_game_pause_menu_gui->init("GUI");
	m_singleplayer_game_pause_menu_gui->loadScheme("AlfiskoSkin.scheme");
	m_singleplayer_game_pause_menu_gui->setFont("DejaVuSans-10");

	CEGUI::PushButton* singleplayer_game_pause_menu_button1 = static_cast<CEGUI::PushButton*>(m_singleplayer_game_pause_menu_gui->createWidget("AlfiskoSkin/Button", "singleplayer_game_pause_menu_button1"));
	m_singleplayer_game_pause_menu_gui->setWidgetPositionPerc(singleplayer_game_pause_menu_button1, 0, 0);
	m_singleplayer_game_pause_menu_gui->setWidgetSizePerc(singleplayer_game_pause_menu_button1, 0.1, 0.05);
	m_singleplayer_game_pause_menu_gui->setWidgetPositionPerc(singleplayer_game_pause_menu_button1, 0.5 - singleplayer_game_pause_menu_button1->getWidth().d_scale / 2.0, 0.45 - singleplayer_game_pause_menu_button1->getHeight().d_scale / 2.0);
	singleplayer_game_pause_menu_button1->setText("resume");
	singleplayer_game_pause_menu_button1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_singleplayer_game, this));


	CEGUI::PushButton* singleplayer_game_pause_menu_button2 = static_cast<CEGUI::PushButton*>(m_singleplayer_game_pause_menu_gui->createWidget("AlfiskoSkin/Button", "singleplayer_game_pause_menu_button2"));
	m_singleplayer_game_pause_menu_gui->setWidgetPositionPerc(singleplayer_game_pause_menu_button2, 0, 0);
	m_singleplayer_game_pause_menu_gui->setWidgetSizePerc(singleplayer_game_pause_menu_button2, 0.1, 0.05);
	m_singleplayer_game_pause_menu_gui->setWidgetPositionPerc(singleplayer_game_pause_menu_button2, 0.5 - singleplayer_game_pause_menu_button2->getWidth().d_scale / 2.0, 0.55 - singleplayer_game_pause_menu_button1->getHeight().d_scale / 2.0);
	singleplayer_game_pause_menu_button2->setText("Quit");
	singleplayer_game_pause_menu_button2->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Game::on_navigate_to_main_menu, this));
}
