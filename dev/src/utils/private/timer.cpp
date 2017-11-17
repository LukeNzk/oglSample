#include "timer.h"
#include <chrono>

namespace helper
{
	template< typename T >
	T GetTime()
	{
		using namespace std::chrono;
		high_resolution_clock::time_point time = high_resolution_clock::now();

		const Int64 ns = time.time_since_epoch().count();

		const T result = static_cast< T >( ns * 1e-9 );
		return result;
	}
}


Timer::Timer()
	: m_start( 0.0 )
{
}

void Timer::Start()
{
	m_start = helper::GetTime< Double >();
}

Double Timer::TimeElapsed() const
{
	return helper::GetTime< Double >() - m_start;
}