#include "GUI.h"
#include "input.h"

CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

GUI::GUI()
{
	m_last_time = 0;
}

void GUI::init(const std::string& resourceDirectory)
{
	//Check if renderer and system were already initialized
	if (m_renderer == nullptr)
	{
		m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

		CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
		rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
		rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
		rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
		rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
		rp->setResourceGroupDirectory("looknfeel", resourceDirectory + "/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
	}

	m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
	m_root_window = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	m_context->setRootWindow(m_root_window);
}

void GUI::destroy()
{
	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

void GUI::draw()
{
	m_renderer->beginRendering();
	m_context->draw();
	m_renderer->endRendering();

	glDisable(GL_SCISSOR_TEST);
}

void GUI::setMouseCursor(const std::string& image_file)
{
	m_context->getMouseCursor().setDefaultImage(image_file);
}

void GUI::showMouseCursor()
{
	m_context->getMouseCursor().show();
}

void GUI::hideMouseCurson()
{
	m_context->getMouseCursor().hide();
}

void GUI::update() {
	unsigned int elapsed;
	if (m_last_time == 0) {
		elapsed = 0;
		m_last_time = SDL_GetTicks();
	}
	else {
		unsigned int next_time = SDL_GetTicks();
		elapsed = next_time - m_last_time;
		m_last_time = next_time;
	}
	m_context->injectTimePulse((float)elapsed / 1000.0f);
}

void GUI::handle_input(SDL_Event& evnt)
{
	CEGUI::utf32 codePoint;
	switch (evnt.type) {
	case SDL_MOUSEMOTION:
		m_context->injectMousePosition(evnt.motion.x, evnt.motion.y);
		break;
	case SDL_KEYDOWN:
		m_context->injectKeyDown(SDLScanCodeToCEGUIKey(evnt.key.keysym.sym));
		break;
	case SDL_KEYUP:
		m_context->injectKeyUp(SDLScanCodeToCEGUIKey(evnt.key.keysym.sym));
		break;
	case SDL_TEXTINPUT:
		codePoint = 0;
		for (int i = 0; evnt.text.text[i] != '\0'; i++) {
			codePoint |= (((CEGUI::utf32)evnt.text.text[i]) << (i * 8));
		}
		m_context->injectChar(codePoint);
		break;
	case SDL_MOUSEBUTTONDOWN:
		m_context->injectMouseButtonDown(SDLButtonToCEGUIButton(evnt.button.button));
		break;
	case SDL_MOUSEBUTTONUP:
		m_context->injectMouseButtonUp(SDLButtonToCEGUIButton(evnt.button.button));
		break;
	}
}

void GUI::mouse_motion(float x, float y)
{
	m_context->injectMousePosition(x, y);
}

void GUI::loadScheme(const std::string& scheme_file)
{
	CEGUI::SchemeManager::getSingleton().createFromFile(scheme_file);
}

CEGUI::Window* GUI::createWidget(const std::string& type, const std::string& name)
{
	CEGUI::Window* new_window = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_root_window->addChild(new_window);
	setWidgetPositionPix(new_window, 0, 0);
	setWidgetSizePix(new_window, 50, 50);
	return new_window;
}

void GUI::setWidgetPositionPix(CEGUI::Window* widget, int x_pix, int y_pix)
{
	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(0, x_pix), CEGUI::UDim(0, y_pix)));
}

void GUI::setWidgetPositionPerc(CEGUI::Window* widget, float x_perc, float y_perc)
{
	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(x_perc, 0), CEGUI::UDim(y_perc, 0)));
}

void GUI::setWidgetSizePix(CEGUI::Window* widget, int width_pix, int height_pix)
{
	widget->setSize(CEGUI::USize(CEGUI::UDim(0, width_pix), CEGUI::UDim(0, height_pix)));
}

void GUI::setWidgetSizePerc(CEGUI::Window* widget, float width_perc, float height_perc)
{
	widget->setSize(CEGUI::USize(CEGUI::UDim(width_perc, 0), CEGUI::UDim(height_perc, 0)));
}

void GUI::setFont(const std::string& font_file)
{
	CEGUI::FontManager::getSingleton().createFromFile(font_file + ".font");
	m_context->setDefaultFont(font_file);
}

CEGUI::GUIContext* GUI::getContext()
{
	return m_context;
}

CEGUI::Key::Scan GUI::SDLScanCodeToCEGUIKey(SDL_Keycode key) {
	using namespace CEGUI;
	switch (key) {
	
	case Input::KEY_BACKSPACE:    return Key::Backspace;
	case Input::KEY_TAB:          return Key::Tab;
	case Input::KEY_RETURN:       return Key::Return;
	case Input::KEY_PAUSE:        return Key::Pause;
	case Input::KEY_ESCAPE:       return Key::Escape;
	case Input::KEY_SPACE:        return Key::Space;
	case Input::KEY_COMMA:        return Key::Comma;
	case Input::KEY_MINUS:        return Key::Minus;
	case Input::KEY_PERIOD:       return Key::Period;
	case Input::KEY_SLASH:        return Key::Slash;
	case Input::KEY_0:            return Key::Zero;
	case Input::KEY_1:            return Key::One;
	case Input::KEY_2:            return Key::Two;
	case Input::KEY_3:            return Key::Three;
	case Input::KEY_4:            return Key::Four;
	case Input::KEY_5:            return Key::Five;
	case Input::KEY_6:            return Key::Six;
	case Input::KEY_7:            return Key::Seven;
	case Input::KEY_8:            return Key::Eight;
	case Input::KEY_9:            return Key::Nine;
	case Input::KEY_KP_COLON:     return Key::Colon;
	case Input::KEY_SEMICOLON:    return Key::Semicolon;
	case Input::KEY_EQUALS:       return Key::Equals;
	case Input::KEY_LEFTBRACKET:  return Key::LeftBracket;
	case Input::KEY_BACKSLASH:    return Key::Backslash;
	case Input::KEY_RIGHTBRACKET: return Key::RightBracket;
	case Input::KEY_A:            return Key::A;
	case Input::KEY_B:            return Key::B;
	case Input::KEY_C:            return Key::C;
	case Input::KEY_D:            return Key::D;
	case Input::KEY_E:            return Key::E;
	case Input::KEY_F:            return Key::F;
	case Input::KEY_G:            return Key::G;
	case Input::KEY_H:            return Key::H;
	case Input::KEY_I:            return Key::I;
	case Input::KEY_J:            return Key::J;
	case Input::KEY_K:            return Key::K;
	case Input::KEY_L:            return Key::L;
	case Input::KEY_M:            return Key::M;
	case Input::KEY_N:            return Key::N;
	case Input::KEY_O:            return Key::O;
	case Input::KEY_P:            return Key::P;
	case Input::KEY_Q:            return Key::Q;
	case Input::KEY_R:            return Key::R;
	case Input::KEY_S:            return Key::S;
	case Input::KEY_T:            return Key::T;
	case Input::KEY_U:            return Key::U;
	case Input::KEY_V:            return Key::V;
	case Input::KEY_W:            return Key::W;
	case Input::KEY_X:            return Key::X;
	case Input::KEY_Y:            return Key::Y;
	case Input::KEY_Z:            return Key::Z;
	case Input::KEY_DELETE:       return Key::Delete;
	case Input::KEY_KP_PERIOD:    return Key::Decimal;
	case Input::KEY_KP_DIVIDE:    return Key::Divide;
	case Input::KEY_KP_MULTIPLY:  return Key::Multiply;
	case Input::KEY_KP_MINUS:     return Key::Subtract;
	case Input::KEY_KP_PLUS:      return Key::Add;
	case Input::KEY_KP_ENTER:     return Key::NumpadEnter;
	case Input::KEY_KP_EQUALS:    return Key::NumpadEquals;
	case Input::KEY_UP:           return Key::ArrowUp;
	case Input::KEY_DOWN:         return Key::ArrowDown;
	case Input::KEY_RIGHT:        return Key::ArrowRight;
	case Input::KEY_LEFT:         return Key::ArrowLeft;
	case Input::KEY_INSERT:       return Key::Insert;
	case Input::KEY_HOME:         return Key::Home;
	case Input::KEY_END:          return Key::End;
	case Input::KEY_PAGEUP:       return Key::PageUp;
	case Input::KEY_PAGEDOWN:     return Key::PageDown;
	case Input::KEY_F1:           return Key::F1;
	case Input::KEY_F2:           return Key::F2;
	case Input::KEY_F3:           return Key::F3;
	case Input::KEY_F4:           return Key::F4;
	case Input::KEY_F5:           return Key::F5;
	case Input::KEY_F6:           return Key::F6;
	case Input::KEY_F7:           return Key::F7;
	case Input::KEY_F8:           return Key::F8;
	case Input::KEY_F9:           return Key::F9;
	case Input::KEY_F10:          return Key::F10;
	case Input::KEY_F11:          return Key::F11;
	case Input::KEY_F12:          return Key::F12;
	case Input::KEY_F13:          return Key::F13;
	case Input::KEY_F14:          return Key::F14;
	case Input::KEY_F15:          return Key::F15;
	case Input::KEY_RSHIFT:       return Key::RightShift;
	case Input::KEY_LSHIFT:       return Key::LeftShift;
	case Input::KEY_RCTRL:        return Key::RightControl;
	case Input::KEY_LCTRL:        return Key::LeftControl;
	case Input::KEY_RALT:         return Key::RightAlt;
	case Input::KEY_LALT:         return Key::LeftAlt;
	case Input::KEY_SYSREQ:       return Key::SysRq;
	case Input::KEY_MENU:         return Key::AppMenu;
	case Input::KEY_POWER:        return Key::Power;
	default:                return Key::Unknown;
	}
}

CEGUI::MouseButton GUI::SDLButtonToCEGUIButton(Uint8 sdlButton) {
	switch (sdlButton) {
	case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
	case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
	case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
	case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
	case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
	}
	return CEGUI::MouseButton::NoButton;
}