#pragma once
#include "types.h"

class Timer
{
public:
	Timer();

	void Start();

	Double TimeElapsed() const;

private:
	Double m_start;
};
