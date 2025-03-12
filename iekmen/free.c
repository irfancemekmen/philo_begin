#include "philo.h"

int	mutex_dest(t_data *data, int flag, int x)
{
	int	i;

	i = -1;
	while (flag >= 1 && ++i < x)
		pthread_mutex_destroy(&data->forks[i]);
	if (flag >= 1)
		free(data->forks);
	if (flag > 2)
		pthread_mutex_destroy(&data->anyone_die);
	if (flag > 3)
		pthread_mutex_destroy(&data->print_mutex);
	if (flag > 4)
		pthread_mutex_destroy(&data->time_t);
	if (flag > 5)
		pthread_mutex_destroy(&data->had_enough_mutex);
	return (1);
}

int	return_func(t_data *data, t_philo *philo, int flag)
{
	if (flag == 1)
		write(2, "Error\n", 6);
	free(data);
	free(philo);
	return (1);
}

// if (data->threads)
// 	free(data->threads);
// destroy_clean(data);
// if (data->states)
// 	free(data->states);
// if (data->meals_eaten)
// 	free(data->meals_eaten);
// if (data->last_meal_time)
// 	free(data->last_meal_time);
// if (philo)
// 	free(philo);
// free(data);
