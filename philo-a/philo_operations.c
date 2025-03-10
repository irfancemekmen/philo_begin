/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsen <bsen@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 19:01:04 by bsen              #+#    #+#             */
/*   Updated: 2024/09/09 14:03:23 by bsen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

void	philos_begining(t_philo *philo)
{
	int	i;

	if (philo->data->nb_philo == 1)
	{
		if (pthread_create(&philo->thread, NULL, (void *)one_philo, philo))
			return ;
		pthread_join(philo->thread, NULL);
		return ;
	}
	i = -1;
	while (++i < philo->data->nb_philo)
	{
		if(pthread_create(&philo[i].thread, NULL, philo_life, &philo[i]))
			return (thread_join(philo, i));
	}
	i = -1;
	while (++i < philo->data->nb_philo)
		pthread_join(philo[i].thread, NULL);
}

void	*philo_life(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	if (p->id % 2 == 1)
		sensitive_usleep(p->data->time_to_eat / 2);
	while (1)
	{
		if (p->data->nb_must_eat != -1 && p->meal_eaten == p->data->nb_must_eat)
		{
			pthread_mutex_lock(&p->data->had_enough_m);
			p->data->nb_eat--;
			pthread_mutex_unlock(&p->data->had_enough_m);
		}
		fork_and_eat(p);
		if (deadcheck(p) == 1)
			break ;
		sleep_and_think(p);
		if (deadcheck(p) == 1)
			break ;
	}
	return (NULL);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
	pthread_mutex_unlock(&philo->data->forks[(philo->id)
		% philo->data->nb_philo]);
}

void	fork_and_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->id - 1]);
	if (deadcheck(philo) == 1)
	{
		pthread_mutex_unlock(&philo->data->forks[philo->id - 1]);
		return ;
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[(philo->id)
		% philo->data->nb_philo]);
	if (deadcheck(philo) == 1)
		return (drop_forks(philo), (void)0);
	print_status(philo, "has taken a fork");
	if (deadcheck(philo) == 1)
		return (drop_forks(philo), (void)0);
	print_status(philo, "is eating");
	sensitive_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->data->time_t);
	pthread_mutex_lock(&philo->data->had_enough_m);
	philo->last_time = get_time();
	philo->meal_eaten++;
	pthread_mutex_unlock(&philo->data->time_t);
	pthread_mutex_unlock(&philo->data->had_enough_m);
	drop_forks(philo);
}

void	sleep_and_think(t_philo *philo)
{
	print_status(philo, "is sleeping");
	sensitive_usleep(philo->data->time_to_sleep);
	if (deadcheck(philo) == 1)
		return ;
	print_status(philo, "is thinking");
}
