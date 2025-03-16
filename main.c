#include "philo.h"

void	cleanup(t_data *data, t_philo *philos)
{
	int	i;

	// Null kontrolü ekleyelim
	if (!data)
		return ;
	i = -1;
	if (data->forks)
	{
		while (++i < data->philosopher_count)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	// Mutex yok kontrolü
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	if (data->threads)
		free(data->threads);
	if (data->states)
		free(data->states);
	if (data->meals_eaten)
		free(data->meals_eaten);
	if (data->last_meal_time)
		free(data->last_meal_time);
	if (philos)
		free(philos);
	// data'yı da serbest bırakalım
	free(data);
}
int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philos;

	if (ac < 5 || ac > 6)
	{
		printf("USAGE:%s<philosopher_number><death_time><eating_time><sleeping_time>[number of meals each_philosopher_needs_to_eat]\n ",
				av[0]);
		return (1);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->threads = NULL;
	data->forks = NULL;
	data->states = NULL;
	data->meals_eaten = NULL;
	data->last_meal_time = NULL;
	if (av_config(av, data) || init_simulation(data))
	{
		cleanup(data, NULL);
		return (1);
	}
	philos = malloc(sizeof(t_philo) * data->philosopher_count);
	if (!philos)
	{
		cleanup(data, NULL);
		return (1);
	}
	if (thread_start(data, philos))
		return (1);
	cleanup(data, philos);
	return (0);
}
