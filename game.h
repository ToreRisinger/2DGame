#pragma once

#include "input.h"
#include "graphics.h"
#include "player.h"
#include "game_level.h"
#include "timer.h"
#include "game_entity.h"
#include "ingame_gui.h"
#include "graphical_user_interface.h"
#include "gui_element_background.h"

#include <vector>
#include <list>

enum GameState {
	MAIN_MENU,
	MULTIPLAYER_MENU,
	SINGLEPLAYER_MENU,
	LOADOUT_MENU,
	STARTING_NEW_SINGLEPLAYER_GAME,
	SINGLEPLAYER_GAME,
	SINGLEPLAYER_GAME_PAUSE_MENU,
	EXIT_GAME

};

class Game
{
public:
	Game();
	~Game();

	void run();

	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;

	//-----GRAPHICS-----
	static Graphics* graphics;

	//-----INPUT-----
	static Input* input;

	//-----PLAYERS-----
	static Player* local_player;
	static std::vector<Player*> players;

	//-----ENTITIES-----
	static std::list<Game_Entity*> entities;

	//----GAME LEVEL-----
	static GameLevel* game_level;
	Game_Level_Option m_game_level_option;

	//-----GAME STATE-----
	static GameState game_state;

private:
	void handle_input(Timer& last_update);
	void on_tick(Timer& last_update);
	void draw_game();
	void remove_dead_entities();

	void start_new_singleplayer_game();

	void center_camera_on_local_player();

	//-----VARIABLES------
	bool m_running;

	void change_game_state(GameState new_game_state);

	void on_navigate_to_multiplayer_menu();
	void on_navigate_to_singleplayer_menu();
	void on_navigate_to_loadout_menu();
	void on_navigate_to_main_menu();
	void on_navigate_to_exit_game();
	void on_navigate_to_starting_new_singleplayer_Game();
	void on_navigate_to_singleplayer_game();
	void on_navigate_to_singleplayer_game_pause_menu();

	//-----NEW GUI-----
	void init_GUI();
	Graphical_User_Interface* m_active_gui;
	Graphical_User_Interface* m_main_menu_gui;
	Graphical_User_Interface* m_multiplayer_menu_gui;
	Graphical_User_Interface* m_singleplayer_menu_gui;
	Graphical_User_Interface* m_loadout_menu_gui;
	Graphical_User_Interface* m_singleplayer_pause_menu_gui;
	Graphical_User_Interface* m_in_game_gui;
};

