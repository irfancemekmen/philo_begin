#include "philo.h"

int	av_config(char **av, t_data *data)
{
	data->philosopher_count = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->simulation_stop = 0;
	data->must_eat_count = -1;
	if (av[5])
		data->must_eat_count = ft_atoi(av[5]);
	if (data->philosopher_count <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0 || (av[5]
			&& data->must_eat_count <= 0))
	{
		printf("Geçersiz argümanlar.\n");
		return (1);
	}
	return (0);
}

int	mutex_initialization(t_data *data)
{
	int	i;
	int	error;
	int	j;

	i = -1;
	error = 0;
	error |= pthread_mutex_init(&data->state_mutex, NULL);
	error |= pthread_mutex_init(&data->stop_mutex, NULL);
	error |= pthread_mutex_init(&data->print_mutex, NULL);
	if (error != 0)
	{
		// Mutex init hatası durumunda belleği temizle
		free(data->threads);
		free(data->forks);
		free(data->states);
		free(data->meals_eaten);
		free(data->last_meal_time);
		return (1);
	}
	data->simulation_start = get_current_time_ms();
	while (++i < data->philosopher_count)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			// Başarısız fork mutex'i için önceki mutex'leri destroy et
			j = -1;
			while (++j < i)
				pthread_mutex_destroy(&data->forks[j]);
			pthread_mutex_destroy(&data->state_mutex);
			pthread_mutex_destroy(&data->stop_mutex);
			pthread_mutex_destroy(&data->print_mutex);
			free(data->threads);
			free(data->forks);
			free(data->states);
			free(data->meals_eaten);
			free(data->last_meal_time);
			return (1);
		}
		data->states[i] = 0;
		data->meals_eaten[i] = 0;
		data->last_meal_time[i] = data->simulation_start;
	}
	return (0);
}

int	init_simulation(t_data *data)
{
	data->threads = malloc(sizeof(pthread_t) * data->philosopher_count);
	if (!data->threads)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philosopher_count);
	if (!data->forks)
	{
		free(data->threads);
		return (1);
	}
	data->states = malloc(sizeof(int) * data->philosopher_count);
	if (!data->states)
	{
		free(data->threads);
		free(data->forks);
		return (1);
	}
	data->meals_eaten = malloc(sizeof(int) * data->philosopher_count);
	if (!data->meals_eaten)
	{
		free(data->threads);
		free(data->forks);
		free(data->states);
		return (1);
	}
	data->last_meal_time = malloc(sizeof(long long) * data->philosopher_count);
	if (!data->last_meal_time)
	{
		free(data->threads);
		free(data->forks);
		free(data->states);
		free(data->meals_eaten);
		return (1);
	}
	// Mevcut kod...
	if (mutex_initialization(data) != 0)
		return (1);
	return (0);
}
