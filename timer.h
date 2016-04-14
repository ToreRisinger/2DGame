#pragma once

class Timer
{
public:
	Timer();
	~Timer();

	void start();
	unsigned int getTime();

private:
	unsigned int _startTime;
};

