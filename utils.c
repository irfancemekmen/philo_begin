#include "philo.h"

long long	get_current_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	ft_sleep(long long ms)
{
	long long	start_time;
	long long	current_time;

	start_time = get_current_time_ms();
	while (1)
	{
		current_time = get_current_time_ms();
		if (current_time - start_time >= ms)
			break ;
		usleep(50); // Daha hassas zaman dilimlerinde kontrol (0.05ms)
	}
}

int	str_contains(const char *str, const char *substr)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (substr[j] && str[i + j] == substr[j])
			j++;
		if (substr[j] == '\0')
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (result);
}
