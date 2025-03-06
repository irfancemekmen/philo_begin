#include "philo.h"

int	init_sim(t_data *data)
{
	int	i;

	data->threads = malloc(sizeof(pthread_t) * data->philo_num);
	if (!data->threads)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (return_func(data, NULL, 1));
	data->can_eat = malloc(sizeof(pthread_cond_t) * data->philo_num);
	if (!data->can_eat)
		return (return_func(data, NULL, 1));
	data->states = malloc(sizeof(int) * data->philo_num);
	if (!data->states)
		return (return_func(data, NULL, 1));
	data->meals_eaten = malloc(sizeof(int) * data->philo_num);
	if (!data->meals_eaten)
		return (return_func(data, NULL, 1));
	data->last_meal_time = malloc(sizeof(long long) * data->philo_num);
	if (!data->last_meal_time)
		return (return_func(data, NULL, 1));
	pthread_mutex_init(&data->state_mutex, NULL);
	data->sim_start = get_current_time();
	i = -1;
	while (++i < data->philo_num)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		pthread_cond_init(&data->can_eat[i], NULL);
		data->states[i] = THINKING;
		data->meals_eaten[i] = 0;
		data->last_meal_time[i] = data->sim_start;
	}
	return (1);
}

int	av_init(t_data *data, char **av)
{
	data->philo_num = atoi(av[1]);
	data->die_time = atoi(av[2]);
	data->eat_time = atoi(av[3]);
	data->sleep_time = atoi(av[4]);
	if (av[5])
		data->must_eat = atoi(av[5]);
	else
		data->must_eat = -1;
	data->sim_stop = 0;
	if (data->philo_num <= 0 || data->die_time <= 0 || data->eat_time <= 0
		|| data->sleep_time <= 0 || (av[5] && data->must_eat <= 0))
		return (1);
	return (0);
}
