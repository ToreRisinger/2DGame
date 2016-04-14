#pragma once

#include "player.h"
#include "timer.h"

class BotPlayer : public Player
{
public:
	BotPlayer();
	~BotPlayer();

	void on_tick(unsigned int delta_time);
	void handle_input(unsigned int delta_time);

private:
	Timer m_jump_timer;
};

