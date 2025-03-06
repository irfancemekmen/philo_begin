#include "philo.h"

void	free_func(t_data *data, int flag)
{
	if (flag > 0)
		free(data->threads);
	if (flag > 1)
		free(data->forks);
	if (flag > 2)
		free(data->can_eat);
	if (flag > 3)
		free(data->states);
	if (flag > 4)
		free(data->meals_eaten);
	if (flag > 5)
		free(data->last_meal_time);
}

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
	if (data->can_eat)
	{
		i = 0;
		while (i < data->philo_num)
			pthread_cond_destroy(&data->can_eat[i++]);
		free(data->can_eat);
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
		printf("Error!\n");
	return (0);
}
