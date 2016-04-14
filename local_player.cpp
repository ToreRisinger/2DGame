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

	//-----------MOVEMENT-----------
	if (m_character->getDirection() == Direction::RIGHT)
	{
		if (m_input->getKeyDown(Input::KEYS::KEY_D))
		{
			x_vel += speed;

			/*
			if (m_input->getKeyPressed(Input::KEYS::KEY_A))
			{
				m_character->useSkill();
			}
			*/
		}
		else if (m_input->getKeyDown(Input::KEYS::KEY_A))
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
		if (m_input->getKeyDown(Input::KEYS::KEY_A))
		{
			x_vel -= speed;

			/*
			if (m_input->getKeyPressed(Input::KEYS::KEY_D))
			{
				m_character->useSkill();
			}
			*/
			
		}
		else if (m_input->getKeyDown(Input::KEYS::KEY_D))
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
	if (m_input->getKeyDown(Input::KEYS::KEY_W))
	{
		m_character->increaseAim(time);
	}

	if (m_input->getKeyDown(Input::KEYS::KEY_S))
	{
		m_character->decreaseAim(time);
	}

	//-----------JUMP-----------
	if (m_input->getKeyPressed(Input::KEYS::KEY_LCTRL))
	{
		if (m_character->isWalking())
		{
			m_character->jump();
		}
	}

	//-----------SHOOT-----------
	if (m_input->getKeyDown(Input::MOUSE::MOUSE_LEFT_BUTTON))
	{
		m_character->use_item();
	}

	//-----------CHANGE SKILL SELECTION-----------
	if (m_input->getKeyPressed(Input::KEYS::KEY_1))
	{
		m_character->select_next_item();
	}

}
