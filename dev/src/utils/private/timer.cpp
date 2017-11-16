#include "timer.h"
#include <chrono>

template class Timer<Float>;
template class Timer<Double>;
template class Timer<Uint32>;
template class Timer<Uint64>;
template class Timer<Int32>;
template class Timer<Int64>;

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


template<typename T>
Timer<T>::Timer()
	: m_start( T( 0 ) )
{
}

template< typename T >
void Timer< T >::Start()
{
	m_start = helper::GetTime< T >();
}

template<typename T>
T Timer<T>::TimeElapsed() const
{
	return helper::GetTime< T >() - m_start;
}
