#include "philo.h"

int	cleanup(t_data *data, t_philo *philos, int error, int flag)
{
	if (error == 1)
		printf("Error!\n");
	if (flag > 0)
		init_destroy(data, flag);
	if (data->threads)
		free(data->threads);
	if (data->forks)
		free(data->forks);
	if (data->states)
		free(data->states);
	if (data->meals_eaten)
		free(data->meals_eaten);
	if (data->last_meal_time)
		free(data->last_meal_time);
	if (philos)
		free(philos);
	free(data);
	return (1);
}

int	init_destroy(t_data *data, int flag)
{
	int	i;

	if (flag > 0)
		pthread_mutex_destroy(&data->state_mutex);
	if (flag > 1)
		pthread_mutex_destroy(&data->stop_mutex);
	if (flag > 2)
		pthread_mutex_destroy(&data->print_mutex);
	if (flag > 3)
	{
		i = -1;
		if (data->forks)
		{
			while (++i < flag - 3)
				pthread_mutex_destroy(&data->forks[i]);
		}
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philos;

	if (ac < 5 || ac > 6)
	{
		printf("Check Arguments!\n");
		return (1);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (cleanup(NULL, NULL, 1, 0));
	if (av_config(av, data) || init_simulation(data))
		return (cleanup(data, NULL, 1, 0));
	philos = malloc(sizeof(t_philo) * data->philosopher_count);
	if (!philos)
		return (cleanup(data, NULL, 1, data->philosopher_count + 3));
	if (thread_start(data, philos))
		return (cleanup(data, philos, 1, data->philosopher_count + 3));
	cleanup(data, philos, 0, data->philosopher_count + 3);
	return (0);
}
