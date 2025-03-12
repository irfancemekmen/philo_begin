#include "philo.h"

int	philo_creat(t_data *data, t_philo *philo)
{
	int	i;
	int	j;

	i = 0;
	while (i < data->philo_num)
	{
		philo[i].philo_id = i + 1;
		philo[i].data = data;
		if (pthread_create(&data->threads[i], NULL, philosopher,
				&philo[i]) != 0)
		{
			data->sim_stop = 1;
			j = 0;
			while (j < i)
			{
				pthread_join(data->threads[j], NULL);
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

int	death_thread_creat(t_data *data)
{
	int	i;

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
		return (1);
	}
	i = 0;
	while (i < data->philo_num)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
	pthread_join(data->death_monitor, NULL);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;

	if (ac < 5 || ac > 6)
		return (return_func(NULL, NULL, 1));
	data = malloc(sizeof(t_data));
	if (!data)
		return (return_func(NULL, NULL, 1));
	if (av_init(data, av) || !init_sim(data))
		return (return_func(data, NULL, 1));
	philo = malloc(sizeof(t_philo) * data->philo_num);
	if (!philo)
		return (return_func(data, NULL, 1));
	if (philo_creat(data, philo) || death_thread_creat(data))
		return (return_func(data, philo, 1));
	return_func(data, philo, 0);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philo;

	if (ac < 5 || ac > 6)
		return (return_func(NULL, NULL, 1));
	data = malloc(sizeof(t_data));
	if (!data)
		return (return_func(NULL, NULL, 1));
	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (return_func(data, NULL, 1));
	if (av_init(data, av))
		return (return_func(data, NULL, 1));
	if (philo_init(data, philo))
		return (return_func(data, philo, 1));
	if (pthread_create(&data->ctrl, NULL, &control, philo))
		return (return_func(data, philo, 1), mutex_dest(data, 6,
				data->philo_num));
	philo_life(philo);
}

void ft_usleep(time_t time)
{
	time_t wake;
	wake = current_time() + time;
	while(current_time()<wake)
		usleep(200);
}

void	one_philo(t_philo *philo)
{
	print_status(philo, "take a fork");
	ft_sleep(philo->data->die_time);
}

void philo_life(t_philo *philo)
{
	int i ;
	if (philo->data->philo_num == 1)
	{
		if (pthread_create(&philo->thread, NULL, (void *) one_philo, philo))
			return ;
		pthread_join(philo->thread, NULL);
		return ;
	}
	i = -1;
	while (++i < philo->data->philo_num)
	{
		if (pthread_create(&philo[i].thread, NULL, philo_life, &philo[i]))
			return(thread_join(philo, i));
	}
	i = -1;
	while (++i < philo->data->philo_num)
		pthread_join(philo[i].thread, NULL);
}

void	*control(void *arg)
{
	t_philo	*philo;
	int		i;

	i = -1;
	philo = (t_philo *)arg;
	while (1)
	{
		while (++i < philo->data->philo_num)
		{
			last_t_control(&philo[i]);
			if (dead_or_alive(&philo[i]))
				return (NULL);
			pthread_mutex_lock(&philo->data->had_enough_mutex);
			if (philo->data->fork_num == 0)
			{
				pthread_mutex_unlock(&philo->data->had_enough_mutex);
				pthread_mutex_lock(&philo->data->anyone_die);
				philo->data->dead = 1;
				return (pthread_mutex_unlock(&philo->data->anyone_die), NULL);
			}
			pthread_mutex_unlock(&philo->data->had_enough_mutex);
		}
		i = 1;
	}
	return (NULL);
}

void	last_time_control(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->time_t);
	if (philo->last_time == 0)
		philo->last_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->time_t);
}

void	whodied(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->anyone_die);
	philo->data->dead = 1;
	pthread_mutex_unlock(&philo->data->anyone_die);
}

int	dead_or_alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->time_t);
	if (philo->data->die_time <= (current_time() - philo->last_time))
	{
		pthread_mutex_unlock(&philo->data->time_t);
		pthread_mutex_lock(&philo->data->anyone_die);
		philo->data->dead = 1;
		pthread_mutex_unlock(&philo->data->anyone_die);
		print_status(philo, "dead");
		return (1);
	}
	pthread_mutex_unlock(&philo->data->time_t);
	return (0);
}

int	philo_init(t_data *data, t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
	{
		philo[i].philo_id = i + 1;
		philo[i].data = data;
		philo[i].meal_eaten = 0;
		philo[i].last_time = 0;
		philo[i].had_enough = 0;
	}
	return (0);
}
