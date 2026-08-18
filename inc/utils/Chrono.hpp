#pragma once

#include <ctime>
#include <iostream>
#include <string>

class	Chrono
{
	public:
		Chrono()
		{
			start();
		}
		~Chrono() {}

		void	start()
		{
			_start = getTime();
		}
		double	get()
		{
			return (getTime() - _start);
		}

		static double getTime()
		{
			double	res;
			struct timespec	current;
			clock_gettime(CLOCK_MONOTONIC, &current);
			res = (current.tv_sec) + (current.tv_nsec) * 1e-9;
			return (res);
		}
	private:
		double		_start = 0;
};
