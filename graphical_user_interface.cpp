#include "graphical_user_interface.h"


Graphical_User_Interface::Graphical_User_Interface()
{

}

Graphical_User_Interface::~Graphical_User_Interface()
{

}

void Graphical_User_Interface::on_tick(unsigned int delta_time)
{
	for (GUI_Element* element : m_elements)
	{
		element->on_tick(delta_time);
	}
}

void Graphical_User_Interface::draw()
{
	for (GUI_Element* element : m_elements)
	{
		element->draw();
	}
}

void Graphical_User_Interface::handle_input()
{
	for (GUI_Element* element : m_elements)
	{
		element->handle_input();
	}
}

void Graphical_User_Interface::add_gui_element(GUI_Element* element)
{
	m_elements.push_back(element);
}

void Graphical_User_Interface::reset()
{
	for (GUI_Element* element : m_elements)
	{
		element->reset();
	}
}