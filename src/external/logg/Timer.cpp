#include "Timer.hh"

#include "StreamOutput.hh"
#include "../logg.hh"


#include <string>


template <class D>
static int __print(logg::Entry & e, logg::Timer::duration & d, const char * unit)
{
	auto tmp = std::chrono::duration_cast<D>(d).count();
	if (tmp >= 1) {
		e.append(std::to_string(tmp));
		e.append(unit);
		d -= D(tmp);
		return (1);
	}
	return (0);
}


static void _print(logg::Entry & e, logg::Timer::duration d)
{
	int i = 0;
	
	i += __print<std::chrono::hours>(e, d, "h");
	i += __print<std::chrono::minutes>(e, d, "m");
	i += __print<std::chrono::seconds     >(e, d, "s");

	if (i < 2)
		i += __print<std::chrono::milliseconds>(e, d, "ms");
	if (i < 2)
		i += __print<std::chrono::microseconds>(e, d, "us");
	if (i < 2)
		i += __print<std::chrono::nanoseconds >(e, d, "ns");
}


template <bool is_steady>
class Warning
{
public:
	Warning(logg::I_Output & out)
	{
		if constexpr (is_steady == false)
			STREAM_LOG_ERROR(out) << "logg::Timer::clock_t is not steady. Sensible to change of system clock.";
	}

	~Warning() = default;
};


/*
                           _______
                          /_  __(_)___ ___  ___  _____
                           / / / / __ `__ \/ _ \/ ___/
                          / / / / / / / / /  __/ /
                         /_/ /_/_/ /_/ /_/\___/_/
*/


logg::Timer::Timer(I_Output & out, char const * file, std::size_t line, char const * func, char const * name)
: _entry(out.log(logg::Profil, file, line, func)),
  _time(),
  _total(duration::zero()),
  _min(duration::max()),
  _max(duration::min()),
  _mean(duration::zero()),
  _count(0)
{
	static const ::Warning<is_steady> w(out);

	if constexpr (is_steady == false)
		_entry.append("*");

	_entry.append(name);
	_entry.append(": ");
}

logg::Timer::~Timer()
{
	_entry.append("min: ");
	_print(_entry, _min);
	_entry.append("\tmax: ");
	_print(_entry, _max);
	_entry.append("\tmean: ");
	_print(_entry, _mean);
	_entry.append("\ttotal: ");
	_print(_entry, _total);
}

void logg::Timer::start()
{
	_time = clock_t::now();
}

void logg::Timer::stop()
{
	++_count;
	auto tmp = clock_t::now() - _time;
	_total += tmp;
	if (_min > tmp)
		_min = tmp;
	if (_max < tmp)
		_max = tmp;
	_mean = duration((_mean.count() * (_count - 1) + tmp.count()) / _count);
}

void logg::Timer::reset_stat()
{
  _total = duration::zero();
  _min   = duration::max();
  _max   = duration::min();
  _mean  = duration::zero();
  _count = 0;
}


/*
                      _____                     _______
                     / ___/_________  ____  ___/_  __(_)___ ___  ___  _____
                     \__ \/ ___/ __ \/ __ \/ _ \/ / / / __ `__ \/ _ \/ ___/
                    ___/ / /__/ /_/ / /_/ /  __/ / / / / / / / /  __/ /
                   /____/\___/\____/ .___/\___/_/ /_/_/ /_/ /_/\___/_/
                                  /_/
*/


logg::ScopeTimer::ScopeTimer(Timer & t)
: _t(t)
{
	_t.start();
}

logg::ScopeTimer::~ScopeTimer()
{
	_t.stop();
}
