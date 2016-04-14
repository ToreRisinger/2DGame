#include "input.h"

Input::Input()
{
	for (int i{}; i < NUM_KEYS; ++i)
	{
		_keysDown[i] = false;
		_keysDownOld[i] = false;
		_keysPressed[i] = false;
	}
}

Input::~Input()
{

}

void Input::handleInput(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		_keysDown[e->key.keysym.scancode] = true;

		if (_keysDownOld[e->key.keysym.scancode] == false)
		{
			_keysPressed[e->key.keysym.scancode] = true;
		}
	}
	else if (e->type == SDL_KEYUP)
	{
		_keysDown[e->key.keysym.scancode] = false;
	}
	else if (e->type == SDL_MOUSEMOTION)
	{
		_mouseRelX = e->motion.xrel;
		_mouseRelY = e->motion.yrel;
		_mouseX = e->motion.x;
		_mouseY = e->motion.y;
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN) {
		if (e->button.button == SDL_BUTTON_LEFT || e->button.button == SDL_BUTTON_RIGHT)
		{
			_keysDown[e->button.button] = true;
		}

		if (_keysDownOld[e->button.button] == false)
		{
			_keysPressed[e->button.button] = true;
		}
	}
	else if (e->type == SDL_MOUSEBUTTONUP) {
		if (e->button.button == SDL_BUTTON_LEFT || e->button.button == SDL_BUTTON_RIGHT)
		{
			_keysDown[e->button.button] = false;
		}
	}

	for (int i{}; i < NUM_KEYS; ++i)
		_keysDownOld[i] = _keysDown[i];
}

void Input::reset()
{
	_mouseRelX = 0;
	_mouseRelY = 0;
	for (int i{}; i < NUM_KEYS; ++i)
		_keysPressed[i] = false;
}

bool Input::getKeyDown(int key)
{
	return _keysDown[key];
}

bool Input::getKeyPressed(int key)
{
	return _keysPressed[key];
}

float Input::getMouseRelX()
{
	return _mouseRelX;
}

float Input::getMouseRelY()
{
	return _mouseRelY;
}

float Input::getMouseX()
{
	return _mouseX;
}

float Input::getMouseY()
{
	return _mouseY;
}