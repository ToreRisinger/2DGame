#include "projectile.h"

#include <iostream>
#include <math.h>
#include <vector>

#include "graphics.h"
#include "player.h"
#include "character.h"

Projectile::Projectile(float x, float y, int width, int height, float x_vel, float y_vel, int hit_damage, int explosion_damage) 
	: Moving_Game_Entity{ x, y, width, height, x_vel, y_vel}
{
	m_projectile_type = NORMAL;

	m_life_time = 0;

	m_exploding_radius = 0;

	m_hit_damage = hit_damage;

	m_explosion_damage = explosion_damage;

	m_gravity = 0;

	m_bounce = 0;

	m_is_stuck = false;

	m_can_hit_player = true;

	m_on_death_registered = false;
	m_on_tick_registered = false;
}

Projectile::~Projectile()
{

}

void Projectile::on_tick(unsigned int delta_time)
{
	//Gravity
	if (m_gravity != 0)
	{
		m_y_vel -= m_gravity * delta_time;

		if (m_y_vel < 0)
		{
			m_rotation = -acos(Normalize(vec2{ m_x_vel, m_y_vel }).x);
		}
		else
		{
			m_rotation = acos(Normalize(vec2{ m_x_vel, m_y_vel }).x);
		}
	}

	//Calculate new position
	float new_x = m_x + m_x_vel * delta_time;
	float new_y = m_y + m_y_vel * delta_time;

	switch (m_projectile_type)
	{
		case NORMAL:

			//Move if not hit something
			if(Game_Object::m_game_level->isSolid(new_x, new_y))
			{
				set_dead(true);
				on_death();
			}
			else 
			{
				//Move forward
				setX(new_x);
				setY(new_y);
			}

			break;
		case BOUNCE:

			//Reduce life time
			m_life_time -= delta_time;

			if (m_life_time <= 0)
			{
				set_dead(true);
				on_death();
			}

			//x_vel fast
			if (m_x_vel * m_x_vel > m_y_vel * m_y_vel)
			{
				if (Game_Object::m_game_level->isSolid(new_x, m_y))
				{
					m_x_vel = -m_x_vel * m_bounce;
					new_x = m_x + m_x_vel * delta_time;
				}
				else if (Game_Object::m_game_level->isSolid(new_x, new_y))
				{
					m_y_vel = -m_y_vel * m_bounce;
					new_y = m_y + m_y_vel * delta_time;
				}
				else
				{
					//Move forward
					setX(new_x);
					setY(new_y);
				}
			}
			//y_vel fast
			else
			{
				if (Game_Object::m_game_level->isSolid(m_x, new_y))
				{
					m_y_vel = -m_y_vel * m_bounce;
					new_y = m_y + m_y_vel * delta_time;
				}
				else if (Game_Object::m_game_level->isSolid(new_x, new_y))
				{
					m_x_vel = -m_x_vel * m_bounce;
					new_x = m_x + m_x_vel * delta_time;
				}
				else
				{
					//Move forward
					setX(new_x);
					setY(new_y);
				}
			}

			break;
		case STICKY:

			//Reduce life time
			m_life_time -= delta_time;

			if (m_life_time <= 0)
			{
				set_dead(true);
				on_death();
			}

			//Only move if not hit something
			if (!m_is_stuck && Game_Object::m_game_level->isSolid(new_x, new_y))
			{
				m_is_stuck = true;
				m_stuck_x = new_x;
				m_stuck_y = new_y;

				m_x_vel = 0.0;
				m_y_vel = 0.0;
			}
			else if (m_is_stuck && Game_Object::m_game_level->isSolid(m_stuck_x, m_stuck_y))
			{
				m_x_vel = 0.0;
				m_y_vel = 0.0;
			}
			else 
			{
				m_is_stuck = false;

				//Move forward
				setX(new_x);
				setY(new_y);
			}

			break;
		default:
			return;
	}

	if (m_can_hit_player) 
	{
		hit_players();
	}
	
}

void Projectile::draw()
{
	Game_Object::m_graphics->render_texture(m_texture_type, m_x, m_y, -0.3, m_width, m_height, getRotation());
}

void Projectile::on_death()
{
	Game_Object::m_game_level->destroyCircle(m_x, m_y, m_exploding_radius);

	hit_players_explosion();

	Game_Object::m_entities->push_front(new Effect(m_x, m_y, m_effect_size, m_max_life_time, m_effect_start_texture, m_time_per_frame, m_number_of_frames));	
}

void Projectile::on_hit_character(Character* character)
{
	character->take_damage(m_hit_damage);
	m_is_dead = true;
	on_death();
}

void Projectile::on_hit_character_with_explosion(Character* character)
{
	character->take_damage(m_explosion_damage);
}

void Projectile::set_explosion_radius(float radius)
{
	m_exploding_radius = radius;
}

float Projectile::get_explosion_radius()
{
	return m_exploding_radius;
}

void Projectile::set_hit_damage(int damage)
{
	m_hit_damage = damage;
}

int Projectile::get_hit_damage()
{
	return m_hit_damage;
}

void Projectile::set_explosion_damage(int damage)
{
	m_explosion_damage = damage;
}

int Projectile::get_explosion_damage()
{
	return m_explosion_damage;
}

void Projectile::register_on_death_callback(std::function<void(Projectile*)> on_death_callback_function)
{
	m_on_death_registered = true;
	m_on_death_callback_function = on_death_callback_function;
}

void Projectile::register_on_tick_callback(std::function<void(Projectile*, unsigned int)> on_tick_callback_function)
{
	m_on_tick_registered = true;
	m_on_tick_callback_function = on_tick_callback_function;
}

void Projectile::set_texture_type(TextureType type)
{
	m_texture_type = type;
}

void Projectile::set_gravity(float value)
{
	m_gravity = value;
}

void Projectile::set_bounce(float bounce)
{
	m_bounce = bounce;
}

void Projectile::set_life_time(int life_time)
{
	m_life_time = life_time;
}

void Projectile::set_projectile_type(Projectile_Type type)
{
	m_projectile_type = type;
}

void Projectile::hit_players()
{
	for (int i = 0; i < m_players->size(); i++)
	{
		if (m_players->at(i)->getCharacter()->point_is_in_character(m_x, m_y))
		{
			on_hit_character(m_players->at(i)->getCharacter());
		}
	}
}

void Projectile::hit_players_explosion()
{
	for (int i = 0; i < m_players->size(); i++)
	{
		if (m_players->at(i)->getCharacter()->point_is_in_range_of_character(m_x, m_y, m_exploding_radius))
		{
			on_hit_character_with_explosion(m_players->at(i)->getCharacter());
		}
	}
}

void Projectile::set_effect(int effect_size, int max_life_time, TextureType effect_start_texture, int time_per_frame, int number_of_frames)
{
	m_effect_start_texture = effect_start_texture;
	m_effect_size = effect_size;
	m_max_life_time = max_life_time;
	m_time_per_frame = time_per_frame;
	m_number_of_frames = number_of_frames;
}

void Projectile::set_can_hit_players(bool value)
{
	m_can_hit_player = value;
}