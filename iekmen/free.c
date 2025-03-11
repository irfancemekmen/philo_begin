#include "philo.h"

void	destroy_clean(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philo_num)
			pthread_mutex_destroy(&data->forks[i++]);
		free(data->forks);
	}
	if (data->eat_mutex)
	{
		i = 0;
		while (i < data->philo_num)
			pthread_mutex_destroy(&data->eat_mutex[i++]);
		free(data->eat_mutex);
	}
	pthread_mutex_destroy(&data->state_mutex);
}

int	return_func(t_data *data, t_philo *philo, int flag)
{
	if (data->threads)
		free(data->threads);
	destroy_clean(data);
	if (data->states)
		free(data->states);
	if (data->meals_eaten)
		free(data->meals_eaten);
	if (data->last_meal_time)
		free(data->last_meal_time);
	if (philo)
		free(philo);
	free(data);
	if (flag == 1)
		write(2, "Error\n", 6);
	return (0);
}
