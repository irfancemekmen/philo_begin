#include "philo.h"

void	check_philosophers(t_data *data, int id)
{
	int	left;
	int	right;

	left = (id + data->philo_num - 1) % data->philo_num;
	right = (id + 1) % data->philo_num;
	if (data->states[id] == SLEEP && data->states[left] != EATING
		&& data->states[right] != EATING)
	{
		data->states[id] = EATING;
		data->last_meal_time[id] = get_current_time();
		pthread_cond_signal(&data->can_eat[id]);
	}
}

void	take_forks(t_philo *philo)
{
	t_data	*data;
	int		id;

	data = philo->data;
	id = philo->philo_id;
	pthread_mutex_lock(&data->state_mutex);
	data->states[id] = SLEEP;
	printf("%lld ms: %d is sleeping.\n", get_current_time() - data->sim_start,
		id + 1);
	check_philosophers(data, id);
	while (data->states[id] != EATING && !data->sim_stop)
		pthread_cond_wait(&data->can_eat[id], &data->state_mutex);
	if (!data->sim_stop)
		printf("%lld ms: %d is eating.\n", get_current_time() - data->sim_start, id
			+ 1);
	pthread_mutex_unlock(&data->state_mutex);
}

void	put_forks(t_philo *philo)
{
	t_data	*data;
	int		id;

	data = philo->data;
	id = philo->philo_id;
	pthread_mutex_lock(&data->state_mutex);
	data->states[id] = THINKING;
	printf("%lld ms: %d is thinking.\n", get_current_time() - data->sim_start,
		id + 1);
	check_philosophers(data, (id + data->philo_num - 1) % data->philo_num);
	check_philosophers(data, (id + 1) % data->philo_num);
	pthread_mutex_unlock(&data->state_mutex);
}

void	*philosopher(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		id;
	int		all_ate_enough;
	int		i;

	philos = (t_philo *)arg;
	data = philos->data;
	id = philos->philo_id;
	if (id % 2 == 0)
		ft_sleep(10);
	while (!data->sim_stop)
	{
		printf("%lld ms: %d is thinking.\n", get_current_time()
			- data->sim_start, id);
		ft_sleep(data->sleep_time);
		if (data->sim_stop)
			break ;
		take_forks(philos);
		if (data->sim_stop)
			break ;
		ft_sleep(data->eat_time);
		pthread_mutex_lock(&data->state_mutex);
		data->meals_eaten[id]++;
		if (data->must_eat > 0 && data->meals_eaten[id] >= data->must_eat)
		{
			all_ate_enough = 1;
			i = 0;
			while (i < data->philo_num)
			{
				if (data->meals_eaten[i] < data->must_eat)
				{
					all_ate_enough = 0;
					break ;
				}
				i++;
			}
			if (all_ate_enough)
			{
				data->sim_stop = 1;
				printf("%lld ms: filolar doydu.\n", get_current_time()
					- data->sim_start);
				pthread_mutex_unlock(&data->state_mutex);
				break ;
			}
		}
		pthread_mutex_unlock(&data->state_mutex);
		put_forks(philos);
	}
	return (NULL);
}

void	*death_monitor_func(void *arg)
{
	t_data		*data;
	long long	current_time;
	int			i;

	data = (t_data *)arg;
	ft_sleep(data->die_time / 2);
	while (!data->sim_stop)
	{
		pthread_mutex_lock(&data->state_mutex);
		current_time = get_current_time();
		i = 0;
		while (i < data->philo_num)
		{
			if (current_time - data->last_meal_time[i] > data->die_time)
			{
				printf("%lld ms: %d is died.\n", current_time - data->sim_start,
					i + 1);
				printf("Last meal: %lld ms\n", current_time
					- data->last_meal_time[i]);
				data->sim_stop = 1;
				pthread_mutex_lock(&data->state_mutex);
				return (NULL);
			}
			i++;
		}
		pthread_mutex_unlock(&data->state_mutex);
		usleep(1000);
	}
	return (NULL);
}

void	cleanup(t_data *data, t_philo *philos)
{
	if (data->threads)
		free(data->threads);
	if (data->forks)
	{
		for (int i = 0; i < data->philo_num; i++)
			pthread_mutex_destroy(&data->forks[i]);
		free(data->forks);
	}
	if (data->can_eat)
	{
		for (int i = 0; i < data->philo_num; i++)
			pthread_cond_destroy(&data->can_eat[i]);
		free(data->can_eat);
	}
	pthread_mutex_destroy(&data->state_mutex);
	if (data->states)
		free(data->states);
	if (data->meals_eaten)
		free(data->meals_eaten);
	if (data->last_meal_time)
		free(data->last_meal_time);
	if (philos)
		free(philos);
	free(data);
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philos;
	int		i;
	int		j;

	if (ac < 5 || ac > 6)
	{
		printf("Check Arguments!\n");
		return (0);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (0);
	if (av_init(data, av))
	{
		printf("Error!\n");
		free(data);
		return (0);
	}
	if (init_sim(data))
	{
		cleanup(data, NULL);
		printf("Simülasyon başlatılamadı!");
		return (0);
	}
	philos = malloc(sizeof(t_philo) * data->philo_num);
	if (!philos)
	{
		cleanup(data, NULL);
		return (1);
	}
	i = 0;
	while (i < data->philo_num)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = data;
		if (pthread_create(&data->threads[i], NULL, philosopher,
				&philos[i]) != 0)
		{
			data->sim_stop = 1;
			j = 0;
			while (j < i)
			{
				pthread_join(data->threads[j], NULL);
				j++;
			}
			cleanup(data, philos);
			printf("thread oluşturulamadı!\n");
			return (0);
		}
		i++;
	}
	if (pthread_create(&data->death_monitor, NULL, death_monitor_func,
			data) != 0)
	{
		data->sim_stop = 1;
		i = 0;
		while (i < data->philo_num)
		{
			pthread_join(data->threads[i], NULL);
			i++;
		}
		cleanup(data, philos);
		printf("ölüm thread oluşturulamadı!\n");
		return (1);
	}
	i = 0;
	while (i < data->philo_num)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(data->death_monitor, NULL);
	cleanup(data, philos);
	return (0);
}
