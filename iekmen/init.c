#include "philo.h"

int	init_sim(t_data *data)
{
	data->threads = malloc(sizeof(pthread_t) * data->philo_num);
	if (!data->threads)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (return_func(data, NULL, 1));
	data->eat_mutex = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->eat_mutex)
		return (return_func(data, NULL, 1));
	data->eat_flags = malloc(sizeof(int) * data->philo_num);
	if (!data->eat_flags)
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
	data->sim_start = current_time();
	mutex_init_func(data);
	return (1);
}

int	mutex_dest(t_data *data, int flag, int x)
{
}

int	mutex_init_func(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (mutex_dest(data, 1, i));
	if (pthread_mutex_init(&data->anyone_die, NULL))
		return (mutex_dest(data, 2, i));
	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (mutex_dest(data, 3, i));
	return (0);
}

int	av_init(t_data *data, char **av)
{
	data->philo_num = ft_atoi(av[1]);
	data->fork_num = ft_atoi(av[1]);
	data->die_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	if (av[5])
		data->must_eat = ft_atoi(av[5]);
	else
		data->must_eat = -1;
	if (data->philo_num <= 0 || data->die_time <= 0 || data->eat_time <= 0
		|| data->sleep_time <= 0 || (av[5] && data->must_eat <= 0))
		return (1);
	data->start_time = current_time();
	data->dead = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		return (1);
	if (mutex_init_func(data))
		return (1);
	return (0);
}
