#ifndef PROF_TIMER_HH__
#define PROF_TIMER_HH__


#include "Entry.hh"


#include <chrono>


namespace logg {
	class I_Output;
} // logg


namespace logg
{


class Timer
{
public:
	using clock_t    = std::chrono::high_resolution_clock;
	using time_point = clock_t::time_point;
	using duration   = clock_t::duration;

	static constexpr bool is_steady = clock_t::is_steady;


	Timer()                     = default;
	Timer(Timer &&)             = default;
	Timer & operator=(Timer &&) = default;

	Timer(I_Output & out, char const * file, std::size_t line, char const * func, char const * name);
	~Timer();

	void start();
	void stop();

	void reset_stat();

private:
	Timer(Timer const &)             = delete;
	Timer & operator=(Timer const &) = delete;

	logg::Entry _entry;

	time_point _time;

	duration _total = duration::zero();
	duration _min = duration::max();
	duration _max = duration::min();
	duration _mean = duration::zero();
	std::size_t _count = 0;
};

class ScopeTimer
{
public:
	ScopeTimer(Timer & t);
	~ScopeTimer();

private:
	Timer & _t;
};


} // logg


#endif // PROF_TIMER_HH__
