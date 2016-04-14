#pragma once

class Character;

class Player
{
public:
	Player();
	~Player();

	virtual void on_tick(unsigned int delta_time);
	virtual void handle_input(unsigned int time);

	Character* getCharacter();
	virtual void set_character(Character* character);

protected:
	Character* m_character;

};

