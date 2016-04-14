#pragma once

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include "math_utils.h"
#include "input.h"

class GUI
{
public:
	GUI();

	void init(const std::string& resourceDirectory);
	void destroy();

	void draw();
	void update();

	void setMouseCursor(const std::string& image_file);
	void showMouseCursor();
	void hideMouseCurson();
	
	void handle_input(SDL_Event& evnt);
	void mouse_motion(float x, float y);

	void loadScheme(const std::string& schemefile);
	void setFont(const std::string& font_file);
	CEGUI::Window* createWidget(const std::string& type, const std::string& name);

	void GUI::setWidgetPositionPix(CEGUI::Window* widget, int x_pix, int y_pix);
	void GUI::setWidgetPositionPerc(CEGUI::Window* widget, float x_perc, float y_perc);
	void GUI::setWidgetSizePix(CEGUI::Window* widget, int width_pix, int height_pix);
	void GUI::setWidgetSizePerc(CEGUI::Window* widget, float width_perc, float height_perc);

	static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
	CEGUI::GUIContext* getContext();

private:
	static CEGUI::OpenGL3Renderer* m_renderer;
	CEGUI::GUIContext* m_context = nullptr;
	CEGUI::Window* m_root_window = nullptr;

	CEGUI::Key::Scan SDLScanCodeToCEGUIKey(SDL_Keycode key);
	CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton);

	unsigned int m_last_time;

};

