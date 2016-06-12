#include "local_player.h"

#include <iostream>

#include "character.h"

LocalPlayer::LocalPlayer(Input* input) : Player()
{
	m_input = input;
}

LocalPlayer::~LocalPlayer()
{

}

void LocalPlayer::on_tick(unsigned int delta_time)
{
	for (int i = 0; i < delta_time; i++)
	{
		m_character->handle_collision();
		m_character->on_tick(1);
	}
}

void LocalPlayer::set_character(Character* character)
{
	if (m_character != nullptr)
	{
		delete m_character;
	}

	m_character = character;
	m_character->set_should_draw_aim(true);
}

void LocalPlayer::handle_input(unsigned int time)
{
	float speed = m_character->getSpeed();
	float x_vel = 0;

	int up = Input::KEYS::KEY_I;
	int down = Input::KEYS::KEY_K;
	int left = Input::KEYS::KEY_J;
	int right = Input::KEYS::KEY_L;

	int jump = Input::KEYS::KEY_SPACE;

	int skill_1 = Input::KEYS::KEY_Q;
	int skill_2 = Input::KEYS::KEY_W;
	int skill_3 = Input::KEYS::KEY_E;
	int skill_4 = Input::KEYS::KEY_R;
	
	//-----------MOVEMENT-----------
	if (m_character->getDirection() == Direction::RIGHT)
	{
		if (m_input->getKeyDown(right))
		{
			x_vel += speed;

		}
		else if (m_input->getKeyDown(left))
		{
			m_character->setDirection(Direction::LEFT);
		}
		else
		{
			x_vel = 0;
		}
	}

	if (m_character->getDirection() == Direction::LEFT)
	{
		if (m_input->getKeyDown(left))
		{
			x_vel -= speed;
			
		}
		else if (m_input->getKeyDown(right))
		{
			m_character->setDirection(Direction::RIGHT);
		}
		else
		{
			x_vel = 0;
		}
	}


	m_character->setXVel(x_vel);

	//-----------AIM-----------
	if (m_input->getKeyDown(up))
	{
		m_character->increaseAim(time);
	}

	if (m_input->getKeyDown(down))
	{
		m_character->decreaseAim(time);
	}

	//-----------JUMP-----------
	if (m_input->getKeyPressed(jump))
	{
		if (m_character->isWalking())
		{
			m_character->jump();
		}
	}

	//-----------SKILLS-----------
	if (m_input->getKeyDown(skill_1))
	{
		m_character->use_skill(1);
	}

	if (m_input->getKeyDown(skill_2))
	{
		m_character->use_skill(2);
	}

	if (m_input->getKeyDown(skill_3))
	{
		m_character->use_skill(3);
	}

	if (m_input->getKeyDown(skill_4))
	{
		m_character->use_skill(4);
	}
}
