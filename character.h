#pragma once

#include <vector>

#include "moving_game_entity.h"
#include "math_utils.h"
#include "animation.h"

class Player;
class Item;

enum Direction {
	LEFT,
	RIGHT
};

class Character : public Moving_Game_Entity
{
public:
	Character(Player* owner);
	~Character();

	void on_tick(unsigned int delta_time);

	void draw();
	void draw_aim_cursor();

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

	void use_item();
	void select_next_item();

	void add_item(Item* item);

	std::vector<Item*>& get_items();

	void set_should_draw_aim(bool value);

	TextureType get_texture_type();

	int get_current_hp();
	void set_current_hp(int hp);

	int get_max_hp();
	void set_max_hp(int hp);

	bool point_is_in_character(float x, float y);
	bool point_is_in_range_of_character(float x, float y, float radius);
	void take_damage(int damage);

	int get_selected_item_index();

private:
	//SKILLS
	std::vector<Item*> m_items;
	int m_item_selected_index;

	//ANIMATION
	Animation m_left_animation;
	Animation m_right_animation;
	Animation* m_active_animation;	

	int m_max_aim;

	bool m_should_draw_aim_cursor;

	//VARIABLES
	Player* m_owner;

	Direction m_direction;

	float m_jump_speed;

	bool m_is_walking;

	int m_walking_counter;

	float m_speed;
	
	float m_aim_degrees;

	float m_aim_speed;

	float m_gravity;

	float m_max_acceleration;

	//CHARACTER PROPERTIES
	int m_current_hp;
	int m_max_hp;
};

