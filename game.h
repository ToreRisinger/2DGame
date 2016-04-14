#pragma once

#include "GUI.h"
#include "input.h"
#include "graphics.h"
#include "player.h"
#include "game_level.h"
#include "timer.h"
#include "game_entity.h"
#include "ingame_gui.h"

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

private:
	void handle_input(Timer& last_update);
	void on_tick(Timer& last_update);
	void draw_game();
	void remove_dead_entities();

	void start_new_singleplayer_game();

	void center_camera_on_local_player();

	//-----VARIABLES------
	bool m_running;
	int m_window_height;
	int m_window_width;
	bool m_debug_mode;

	//-----GRAPHICS-----
	Graphics* m_graphics;

	//-----INPUT-----
	Input* m_input;

	//-----PLAYERS-----
	Player* m_local_player;
	std::vector<Player*> m_players;

	//-----ENTITIES-----
	std::list<Game_Entity*> m_entities;

	//----GAME LEVEL-----
	GameLevel* m_game_level;

	//-----GAME STATE-----
	GameState m_game_state;

	void change_game_state(GameState new_game_state);

	bool on_navigate_to_multiplayer_menu(const CEGUI::EventArgs& e);
	bool on_navigate_to_singleplayer_menu(const CEGUI::EventArgs& e);
	bool on_navigate_to_loadout_menu(const CEGUI::EventArgs& e);
	bool on_navigate_to_main_menu(const CEGUI::EventArgs& e);
	bool on_navigate_to_exit_game(const CEGUI::EventArgs& e);
	bool on_navigate_to_starting_new_singleplayer_Game(const CEGUI::EventArgs& e);
	bool on_navigate_to_singleplayer_game(const CEGUI::EventArgs& e);
	bool on_navigate_to_singleplayer_game_pause_menu(const CEGUI::EventArgs& e);

	//-----GUI-----
	void initGUI();

	GUI* m_active_gui;
	GUI *m_main_menu_gui;
	GUI *m_multiplayer_menu_gui;
	GUI *m_singleplayer_menu_gui;	
	GUI *m_loadout_menu_gui;

	GUI *m_cursor_gui;

	GUI *m_singleplayer_game_gui;
	GUI *m_singleplayer_game_pause_menu_gui;

	//-----INGAME GUI-----
	Ingame_GUI* m_ingame_GUI;
};

