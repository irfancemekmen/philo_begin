#include "philo.h"

long long	current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_sleep(long long ms)
{
	long long	start_time;
	long long	cur_time;

	start_time = current_time();
	while (1)
	{
		cur_time = current_time();
		if (cur_time - start_time >= ms)
			break ;
		usleep(1000);
	}
}
