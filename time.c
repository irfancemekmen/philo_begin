#include "philo.h"

long long	get_current_time(void)
{
	struct timeval	time;

	gettimeday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_sleep(long long ms)
{
	long long	start_time;
	long long	current_time;

	start_time = get_current_time();
	while (1)
	{
		current_time = get_current_time();
		if (current_time - start_time >= ms)
			break ;
		usleep(1000);
	}
}
