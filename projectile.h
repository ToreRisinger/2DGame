#pragma once

#include <functional>

#include "moving_game_entity.h"
#include "math_utils.h"
#include "effect.h"

enum Projectile_Type {
	NORMAL,
	BOUNCE,
	STICKY
};

class Projectile : public Moving_Game_Entity
{
public:
	Projectile(float x, float y, int width, int height, float x_vel, float y_vel, int damage, int explosion_damage);
	~Projectile();

	void on_tick(unsigned int delta_time);
	void draw();
	void on_death();

	void set_texture_type(TextureType type);

	void set_projectile_type(Projectile_Type type);

	void set_explosion_radius(float radius);

	float get_explosion_radius();

	void set_hit_damage(int damage);
	int get_hit_damage();

	void set_explosion_damage(int damage);
	int get_explosion_damage();

	void set_gravity(float gravity);

	void set_bounce(float bounce);

	void set_life_time(int life_time);

	void damage_players();

	void set_effect(int effect_size, int max_life_time, TextureType effect_start_texture, int time_per_frame, int number_of_frames);

	void set_can_hit_players(bool value);

	//REGISTER CALLBACK
	void register_on_death_callback(std::function<void(Projectile*)> on_death_callback_function);
	void register_on_tick_callback(std::function<void(Projectile*, unsigned int)> m_on_tick_callback_function);

protected:
	//CALLBACKS
	std::function<void(Projectile*)> m_on_death_callback_function;
	std::function<void(Projectile*, unsigned int)> m_on_tick_callback_function;

	bool m_on_death_registered;
	bool m_on_tick_registered;

	void on_hit_character(Character* character);
	void on_hit_character_with_explosion(Character* character);
	void hit_players();
	void hit_players_explosion();
	
	//TEXTURE TYPE
	TextureType m_texture_type;

	//PROJECTILE TYPE
	Projectile_Type m_projectile_type;

	float m_exploding_radius;

	int m_hit_damage;
	int m_explosion_damage;

	int m_life_time;

	float m_gravity;

	float m_bounce;

	float m_stuck_x;
	float m_stuck_y;

	bool m_is_stuck;

	bool m_can_hit_player;
	//Effect
	TextureType m_effect_start_texture;
	int m_effect_size;
	int m_max_life_time;
	int m_time_per_frame;
	int m_number_of_frames;
};

