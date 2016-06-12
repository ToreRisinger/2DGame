#pragma once

#include "gui_element.h"
#include "graphics.h"

class GUI_Element_Background : public GUI_Element
{
public:
	GUI_Element_Background(TextureType background_texture, int x, int y, int width, int height);
	~GUI_Element_Background();

	void draw();

private:
	TextureType m_background_texture;

};

