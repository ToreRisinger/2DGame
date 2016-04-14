#include "Timer.h"

#include <SDL/SDL.h>

Timer::Timer()
{

}

Timer::~Timer()
{
}

void Timer::start()
{
	_startTime = SDL_GetTicks();
}

unsigned int Timer::getTime()
{
	return SDL_GetTicks() - _startTime;
}
