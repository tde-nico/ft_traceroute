#include "ft_traceroute.h"

struct timeval	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv);
}

struct timeval	secs2time(double secs)
{
	struct timeval	tv;

	tv.tv_sec = secs;
	tv.tv_usec = (int)((secs - tv.tv_sec) * 1000000);
	return (tv);
}

