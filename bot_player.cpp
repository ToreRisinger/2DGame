#include "bot_player.h"

#include "character.h"

#include <iostream>

BotPlayer::BotPlayer() : Player()
{
	m_jump_timer.start();
}

BotPlayer::~BotPlayer()
{

}

void BotPlayer::on_tick(unsigned int delta_time)
{
	for (int i = 0; i < delta_time; i++)
	{
		m_character->on_tick(1);
		m_character->handle_collision();
	}
}

void BotPlayer::handle_input(unsigned int delta_time)
{
	//TODO AI
	if (m_jump_timer.getTime() >= 2000)
	{
		if (m_character->isWalking())
		{
			m_character->jump();
			m_jump_timer.start();
		}
	}
}
