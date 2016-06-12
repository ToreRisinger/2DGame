#pragma once

#include <vector>

#include "moving_game_entity.h"
#include "math_utils.h"
#include "animation.h"

class Player;
class Item;
class Weapon;
class Skill;

enum Direction {
	LEFT,
	RIGHT
};

class Character : public Moving_Game_Entity
{
public:
	Character();
	~Character();

	void on_tick(unsigned int delta_time);

	void draw();

	void handle_collision();

	void set_start_location(float x, float y);

	float getJumpSpeed();
	void setJumpSpeed(float jump_speed);

	void jump();

	void setWalking(bool value);
	bool isWalking();

	float getSpeed();
	void setSpeed(float speed);

	Direction getDirection();
	void setDirection(Direction direction);

	vec2 getForwardVector();

	float getAim();
	void increaseAim(unsigned int time);
	void decreaseAim(unsigned int time);

	void use_skill(int skill_number);

	void set_should_draw_aim(bool value);

	TextureType get_texture_type();

	int get_current_hp();
	void set_current_hp(int hp);

	int get_max_hp();
	void set_max_hp(int hp);

	bool point_is_in_character(float x, float y);
	bool point_is_in_range_of_character(float x, float y, float radius);
	void take_damage(int damage);

	void set_owner(Player* owner);

	vec2 get_weapon_position();

	Weapon* get_weapon_model();

	std::vector<Skill*>* get_skills();

protected:

	//ANIMATION
	Animation m_left_animation;
	Animation m_right_animation;
	Animation* m_active_animation;	
	bool m_should_draw_aim_cursor;

	//VARIABLES
	Player* m_owner;
	Direction m_direction;
	bool m_is_walking;
	int m_walking_counter;
	float m_gravity;
	float m_max_acceleration;
	int m_max_aim;

	//CHARACTER PROPERTIES
	int m_current_hp;
	int m_max_hp;
	float m_aim_degrees;
	float m_aim_speed;
	float m_jump_speed;
	float m_speed;

	//WEAPON
	Weapon* m_weapon_model;

	//SKILLS
	std::vector<Skill*> m_skills;
};

