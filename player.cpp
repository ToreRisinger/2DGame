#include "player.h"

#include "character.h"

Player::Player()
{
	m_character = nullptr;
}

Player::~Player()
{
	delete m_character;
}

void Player::on_tick(unsigned int delta_time)
{

}

void Player::handle_input(unsigned int time)
{
	
}

Character* Player::getCharacter()
{
	return m_character;
}

void Player::set_character(Character* character)
{
	if (m_character != nullptr) 
	{
		delete m_character;
	}

	m_character = character;
}