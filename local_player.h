#pragma once

#include "player.h"
#include "input.h"

class LocalPlayer : public Player
{
public:
	LocalPlayer(Input* input);
	~LocalPlayer();

	void on_tick(unsigned int delta_time);

	void handle_input(unsigned int time);

	void set_character(Character* character);

private:

	Input* m_input;
};

