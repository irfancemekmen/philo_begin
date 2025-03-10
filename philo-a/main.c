/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsen <bsen@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:43:31 by bsen              #+#    #+#             */
/*   Updated: 2024/09/09 14:08:16 by bsen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

int	init_data(t_data *data, char **av)
{
	data->nb_philo = ft_atoi(av[1]);
	data->nb_eat = ft_atoi(av[1]);
	if (data->nb_philo < 1)
		return (1);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->nb_must_eat = ft_atoi(av[5]);
	else
		data->nb_must_eat = -1;
	data->start_time = get_time();
	data->dead = false;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	if (m_init(data))
		return (1);
	return (0);
}

int	init_philo(t_philo *philo, t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].data = data;
		philo[i].meal_eaten = 0;
		philo[i].last_time = 0;
		philo[i].had_enough = 0;
	}
	return (0);
}

int	check_args(char **av, int ac)
{
	int	j;
	int	i;

	if (ac < 5 || ac > 6)
		return (1);
	j = 0;
	i = 1;
	while (av[i])
	{
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (1);
			j++;
		}
		j = 0;
		i++;
	}
	return (0);
}

void	*control(void *arg)
{
	t_philo	*p;
	int		i;

	i = -1;
	p = (t_philo *)arg;
	while (1)
	{
		while (++i < p->data->nb_philo)
		{
			last_t_control(&p[i]);
			if (dead_or_alive(&p[i]))
				return (NULL);
			pthread_mutex_lock(&p->data->had_enough_m);
			if (p->data->nb_eat == 0)
			{
				pthread_mutex_unlock(&p->data->had_enough_m);
				pthread_mutex_lock(&p->data->anyone_dead_m);
				p->data->dead = true;
				return (pthread_mutex_unlock(&p->data->anyone_dead_m), NULL);
			}
			pthread_mutex_unlock(&p->data->had_enough_m);
		}
		i = -1;
	}
	return (NULL);
}

void	thread_join(t_philo *philos, int len)
{
	int	i;

	i = -1;
	while (--len >= 0)
		pthread_join(philos[i].thread, NULL);
}

int	main(int ac, char **av)
{
	t_data		data;
	t_philo		*philo;
	pthread_t	ctrl;

	if (check_args(av, ac) || init_data(&data, av))
		return (write(2, "Error\n", 6), 1);
	if (data.nb_must_eat <= 0 || data.time_to_die <= 0 || data.time_to_eat <= 0
		|| data.time_to_sleep <= 0 || (av[5] && data.nb_must_eat <= 0))
		return (0);
	philo = malloc(sizeof(t_philo) * data.nb_philo);
	if (!philo)
		return (write(2, "Error\n", 6), mutex_free(&data, 6, data.nb_philo), 1);
	if (init_philo(philo, &data) || pthread_create(&ctrl, NULL, &control,
			philo))
		return (write(2, "Error\n", 6), mutex_free(&data, 6, data.nb_philo),
			free(philo), 1);
	philos_begining(philo);
	clean_data(&data, philo);
	pthread_join(ctrl, NULL);
	return (0);
}
