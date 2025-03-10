/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsen <bsen@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 13:51:10 by bsen              #+#    #+#             */
/*   Updated: 2024/07/03 16:32:42 by bsen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sum;
	int	sign;

	sign = 1;
	sum = 0;
	while ((*(char *)str <= 13 && *(char *)str >= 9) || *(char *)str == ' ')
		str++;
	if (*(char *)str == '+' || *(char *)str == '-')
	{
		if (*(char *)str == '-')
			sign = -1;
		str++;
	}
	while (*(char *)str >= '0' && *(char *)str <= '9')
	{
		sum = (*(char *)str - '0') + (sum * 10);
		str++;
	}
	return (sum * sign);
}

int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->print_m);
	pthread_mutex_lock(&philo->data->anyone_dead_m);
	pthread_mutex_lock(&philo->data->time_t);
	printf("%lu %d %s\n", get_time() - philo->data->start_time, philo->id,
		status);
	pthread_mutex_unlock(&philo->data->time_t);
	pthread_mutex_unlock(&philo->data->anyone_dead_m);
	pthread_mutex_unlock(&philo->data->print_m);
}

void	clean_data(t_data *data, t_philo *philo)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&data->had_enough_m);
	pthread_mutex_destroy(&data->print_m);
	pthread_mutex_destroy(&data->anyone_dead_m);
	i = -1;
	while (++i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i]);
	free(data->forks);
	free(philo);
}

int	dead_or_alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->time_t);
	if (philo->data->time_to_die <= (get_time() - philo->last_time))
	{
		pthread_mutex_unlock(&philo->data->time_t);
		pthread_mutex_lock(&philo->data->anyone_dead_m);
		philo->data->dead = true;
		pthread_mutex_unlock(&philo->data->anyone_dead_m);
		print_status(philo, "died");
		return (1);
	}
	pthread_mutex_unlock(&philo->data->time_t);
	return (0);
}
