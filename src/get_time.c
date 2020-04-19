#include "ft_ping.h"

size_t get_time(void)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
}
