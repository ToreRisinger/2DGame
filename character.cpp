#include "character.h"

#include <iostream>
#include <math.h>
#include "player.h"
#include "graphics.h"
#include "item.h"
#include "shovel.h"
#include "rocket_launcher.h"
#include "pistol.h"
#include "rifle.h"
#include "grenade.h"
#include "sticky_grenade.h"
#include "game.h"
#include "math_utils.h"
#include "particle_system.h"
#include "weapon.h"
#include "skill.h"

Character::Character() : 
Moving_Game_Entity(0, 0, 20, 32, 0, 0),
m_left_animation(TextureType::CHARACTER_SOLDIER_LEFT_FRAME1, 100, 4),
m_right_animation(TextureType::CHARACTER_SOLDIER_RIGHT_FRAME1, 100, 4)
{
	m_active_animation = &m_right_animation;

	m_max_aim = 65;

	//Init variables
	m_direction = Direction::RIGHT;

	m_jump_speed = 0.3;
	m_is_walking = false;
	m_walking_counter = 0;
	m_speed = 0.15;

	m_gravity = 0.00045;
	m_max_acceleration = 2.0;

	m_aim_degrees = 0;
	m_aim_speed = 0.2;

	m_should_draw_aim_cursor = false;

	m_max_hp = 100;
	m_current_hp = m_max_hp;

	m_weapon_model = new Weapon();
	m_weapon_model->set_owner(this);

}

Character::~Character()
{
	m_skills.clear();
}

void Character::on_tick(unsigned int delta_time)
{
	//SKILLS
	for (int i = 0; i < m_skills.size(); i++) 
	{
		m_skills.at(i)->on_tick(delta_time);
	}

	//on_tick animation if walking
	if (m_x_vel != 0)
	{
		m_active_animation->on_tick(delta_time);
	}

	//Make it possible to jump even if not walking for some time
	if (m_is_walking)
	{
		m_walking_counter++;
		if (m_walking_counter >= 200)
		{
			m_is_walking = false;
			m_walking_counter = 0;
		}
	}
	else
	{
		m_walking_counter = 0;
	}

	m_weapon_model->on_tick(delta_time);
}

void Character::draw()
{
	Game::graphics->render_texture(m_active_animation->get_texture_type(), m_x + m_width / 2, m_y + m_height / 2, -0.1, m_width, m_height, 0);

	m_weapon_model->draw();

	if (m_should_draw_aim_cursor)
	{
		m_weapon_model->draw_aim_cursor();
	}
	
}

void Character::handle_collision()
{
	int game_level_min_x = 0;
	int game_level_max_x = Game::game_level->getWidth() * 4;

	int game_level_min_y = 0;
	int game_level_max_y = Game::game_level->getHeight() * 4;

	bool walking = false;

	//-----Accelerate with gravity-----
	m_y_vel -= m_gravity;

	//-----Adjust max acceleration-----
	if (m_y_vel > m_max_acceleration) {
		m_y_vel = m_max_acceleration;
	}
	else if (m_y_vel < 0 && m_y_vel < -m_max_acceleration) {
		m_y_vel = -m_max_acceleration;
	}

	float new_x = m_x + m_x_vel;
	float new_y = m_y + m_y_vel;

	
	//PLAYER WALKS ON DIRT
	if (Game::game_level->isSolid(m_x, new_y) ||
		Game::game_level->isSolid(m_x + 4, new_y) ||
		Game::game_level->isSolid(m_x + 8, new_y) ||
		Game::game_level->isSolid(m_x + 12, new_y) ||
		Game::game_level->isSolid(m_x + 16, new_y) ||
		Game::game_level->isSolid(m_x + 20, new_y))
	{
		setWalking(true);
		
		if (m_y_vel < 0.0)
		{
			m_y = (int)new_y + 1;
			new_y = m_y;
			setYVel(0);
		}
	}

	//PLAYER WALKS ON FLOOR
	if (new_y < game_level_min_y)
	{
		setWalking(true);

		if (m_y_vel < 0.0)
		{
			m_y = game_level_min_y;
			new_y = m_y;
			setYVel(0);
		}
	}

	////---------X AXIS COLLISION---------
	
	//If player traveling to the left
	if (m_x_vel < 0)
	{

		//If player hits left wall
		if (new_x < game_level_min_x)
		{
			new_x = game_level_min_x;
		}
		//Is walking
		else if (isWalking())
		{

			if (!Game::game_level->isSolid(new_x, m_y + 8) &&
				!Game::game_level->isSolid(new_x, m_y + 12) &&
				!Game::game_level->isSolid(new_x, m_y + 16) &&
				!Game::game_level->isSolid(new_x, m_y + 20) &&
				!Game::game_level->isSolid(new_x, m_y + 24)
				)
			{

				int steps = Game::game_level->isSolid(new_x, m_y) +
					Game::game_level->isSolid(new_x, m_y) *
					Game::game_level->isSolid(new_x, m_y + 4);

				m_y += steps * 4;


				//If player hits ceiling
				if (m_y + m_height + 1 > game_level_max_y)
				{
					m_y = game_level_max_y - m_height;
					new_x = m_x;
					setYVel(0);
				}
				else
				{
					new_y = m_y + m_y_vel;
				}

			}
			else
			{
				new_x = m_x;
				setXVel(0);
			}
		}
		//Not walking
		else
		{
			if (Game::game_level->isSolid(new_x, m_y) ||
				Game::game_level->isSolid(new_x, m_y + 4) ||
				Game::game_level->isSolid(new_x, m_y + 8) ||
				Game::game_level->isSolid(new_x, m_y + 12) ||
				Game::game_level->isSolid(new_x, m_y + 16) ||
				Game::game_level->isSolid(new_x, m_y + 20) ||
				Game::game_level->isSolid(new_x, m_y + 24) ||
				Game::game_level->isSolid(new_x, m_y + 28) ||
				Game::game_level->isSolid(new_x, m_y + 32)
				)
			{
				new_x = m_x;
				setXVel(0);
			}
		}

	}

	
	//If player traveling to the right
	else if (m_x_vel > 0)
	{
		//If player hits right wall
		if (new_x + m_width > game_level_max_x)
		{
			new_x = game_level_max_x - m_width;
		}//Is walking
		else if (isWalking())
		{
			if (!Game::game_level->isSolid(new_x + m_width, m_y + 8) &&
				!Game::game_level->isSolid(new_x + m_width, m_y + 12) &&
				!Game::game_level->isSolid(new_x + m_width, m_y + 16) &&
				!Game::game_level->isSolid(new_x + m_width, m_y + 20) &&
				!Game::game_level->isSolid(new_x + m_width, m_y + 24)
				)
			{
				int steps = Game::game_level->isSolid(new_x + m_width, m_y) +
					Game::game_level->isSolid(new_x + m_width, m_y) *
					Game::game_level->isSolid(new_x + m_width, m_y + 4);

				m_y += steps * 4;
			
				//If player hits ceiling
				if (m_y + m_height + 1 > game_level_max_y)
				{
					
					m_y = game_level_max_y - m_height;
					new_x = m_x;
					setYVel(0);
				}
				else
				{
					new_y = m_y + m_y_vel;
				}
			}
			else
			{
				new_x = m_x;
				setXVel(0);
			}
		}
		//Not walking
		else
		{
			if (Game::game_level->isSolid(new_x + m_width, m_y) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 4) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 8) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 12) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 16) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 20) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 24) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 28) ||
				Game::game_level->isSolid(new_x + m_width, m_y + 32)
				)
			{
				new_x = m_x;
				setXVel(0);
			}
		}
	}

	//If player traveling up
	if (m_y_vel > 0)
	{
		//If player hits ceiling
		if (new_y + m_height / 2 > game_level_max_y)
		{
			new_y = game_level_max_y - m_height / 2;
			setYVel(0);
		}

		if (Game::game_level->isSolid(new_x, new_y + m_height) ||
			Game::game_level->isSolid(new_x + 4, new_y + m_height) ||
			Game::game_level->isSolid(new_x + 8, new_y + m_height) ||
			Game::game_level->isSolid(new_x + 12, new_y + m_height) ||
			Game::game_level->isSolid(new_x + 16, new_y + m_height) ||
			Game::game_level->isSolid(new_x + 20, new_y + m_height)
			)
		{
			new_y = m_y;
			setYVel(0);
		}
	}
	
	setX(new_x);
	setY(new_y);
}

void Character::jump()
{
	m_y_vel = m_jump_speed;
	m_is_walking = false;
	m_walking_counter = 0;
}

float Character::getJumpSpeed()
{
	return m_jump_speed;
}

void Character::setJumpSpeed(float jump_speed)
{
	m_jump_speed = jump_speed;
}

void Character::setWalking(bool value)
{
	m_is_walking = value;
}


bool Character::isWalking()
{
	return m_is_walking;
}

Direction Character::getDirection()
{
	return m_direction;
}

void Character::setDirection(Direction direction)
{
	if (direction == LEFT)
	{
		m_left_animation.reset();
		m_active_animation = &m_left_animation;
	}
	else 
	{
		m_right_animation.reset();
		m_active_animation = &m_right_animation;
	}
	
	m_direction = direction;
}


float Character::getAim()
{
	return m_aim_degrees * m_direction - 180 * !m_direction - m_aim_degrees * !m_direction;
}

void Character::increaseAim(unsigned int time)
{
	m_aim_degrees += m_aim_speed * time;

	if (m_aim_degrees > m_max_aim) {
		m_aim_degrees = m_max_aim;
	}
}

void Character::decreaseAim(unsigned int time)
{
	m_aim_degrees -= m_aim_speed * time;

	if (m_aim_degrees < -m_max_aim) {
		m_aim_degrees = -m_max_aim;
	}
}

void Character::use_skill(int skill_number)
{
	
	if (skill_number <= m_skills.size() && m_skills.at(skill_number - 1)->is_off_cooldown())
	{
		m_skills.at(skill_number - 1)->use();
	}
}

vec2 Character::getForwardVector()
{
	if (m_direction == Direction::RIGHT)
	{
		return Normalize(vec2(cos(m_aim_degrees * M_PI / 180.0), sin(m_aim_degrees * M_PI / 180.0)));
	}
	else if (m_direction == Direction::LEFT)
	{
		return Normalize(vec2(-cos(m_aim_degrees * M_PI / 180.0), sin(m_aim_degrees * M_PI / 180.0)));
	}
}

float Character::getSpeed()
{
	return m_speed;
}

void Character::setSpeed(float speed)
{
	m_speed = speed;
}

TextureType Character::get_texture_type()
{
	return m_active_animation->get_texture_type();
	
}

void Character::set_start_location(float x, float y)
{
	m_x = x;
	m_y = y;

	m_weapon_model->on_start_use();
}

void Character::set_should_draw_aim(bool value)
{
	m_should_draw_aim_cursor = value;
}

int Character::get_current_hp()
{
	return m_current_hp;
}

void Character::set_current_hp(int hp)
{
	m_current_hp = hp;
}

int Character::get_max_hp()
{
	return m_max_hp;
}

void Character::set_max_hp(int hp)
{
	m_max_hp = hp;
}

bool Character::point_is_in_character(float x, float y)
{
	vec3 point{ x, y, 0 };

	vec3 hit_sphere_1_pos{ m_x + 10, m_y + 8, 0 };
	vec3 hit_sphere_2_pos{ m_x + 10, m_y + 16, 0 };

	float hit_sphere_radius = 12;

	if (Length(VectorSub(point, hit_sphere_1_pos)) <= hit_sphere_radius)
	{
		return true;
	}
	
	if (Length(VectorSub(point, hit_sphere_2_pos)) <= hit_sphere_radius)
	{
		return true;
	}

	return false;
}

bool Character::point_is_in_range_of_character(float x, float y, float radius)
{
	vec3 point{ x, y, 0 };

	vec3 hit_sphere_1_pos{ m_x + 10, m_y + 8, 0 };
	vec3 hit_sphere_2_pos{ m_x + 10, m_y + 16, 0 };

	float hit_sphere_radius = 12;

	if (Length(VectorSub(point, hit_sphere_1_pos)) <= hit_sphere_radius + radius)
	{
		return true;
	}

	if (Length(VectorSub(point, hit_sphere_2_pos)) <= hit_sphere_radius + radius)
	{
		return true;
	}

	return false;
}

void Character::take_damage(int damage)
{
	m_current_hp -= damage;

	if (m_current_hp <= 0)
	{
		m_current_hp = 0;

		m_is_dead = true;
	}
}

void Character::set_owner(Player* owner)
{
	m_owner = owner;
}

vec2 Character::get_weapon_position()
{
	return m_weapon_model->get_weapon_position();
}

Weapon* Character::get_weapon_model()
{
	return m_weapon_model;
}

std::vector<Skill*>* Character::get_skills()
{
	return &m_skills;
}