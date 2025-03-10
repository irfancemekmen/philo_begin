/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_operations2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsen <bsen@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:21:37 by bsen              #+#    #+#             */
/*   Updated: 2024/09/05 13:21:53 by bsen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sensitive_usleep(time_t time)
{
	time_t	wake_up;

	wake_up = get_time() + time;
	while (get_time() < wake_up)
		usleep(200);
}

int	deadcheck(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->anyone_dead_m);
	if (philo->data->dead == true)
	{
		pthread_mutex_unlock(&philo->data->anyone_dead_m);
		return (1);
	}
	pthread_mutex_lock(&philo->data->had_enough_m);
	if (philo->data->nb_eat == 0)
	{
		pthread_mutex_unlock(&philo->data->had_enough_m);
		pthread_mutex_unlock(&philo->data->anyone_dead_m);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->had_enough_m);
	pthread_mutex_unlock(&philo->data->anyone_dead_m);
	return (0);
}

void	last_t_control(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->time_t);
	if (philo->last_time == 0)
		philo->last_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->time_t);
}

void	whodied(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->anyone_dead_m);
	philo->data->dead = true;
	pthread_mutex_unlock(&philo->data->anyone_dead_m);
}

int	m_init(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philo)
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (mutex_free(data, 1, i));
	if (pthread_mutex_init(&data->anyone_dead_m, NULL))
		return (mutex_free(data, 2, i));
	if (pthread_mutex_init(&data->print_m, NULL))
		return (mutex_free(data, 3, i));
	if (pthread_mutex_init(&data->time_t, NULL))
		return (mutex_free(data, 4, i));
	if (pthread_mutex_init(&data->had_enough_m, NULL))
		return (mutex_free(data, 5, i));
	return (0);
}
