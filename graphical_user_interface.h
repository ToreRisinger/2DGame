#pragma once

#include "gui_element.h"

#include <vector>

class Graphical_User_Interface
{
public:
	Graphical_User_Interface();
	~Graphical_User_Interface();

	void on_tick(unsigned int delta_time);
	void draw();
	void handle_input(); 

	void reset();

	void add_gui_element(GUI_Element* element);

private:
	std::vector<GUI_Element*> m_elements;
};

