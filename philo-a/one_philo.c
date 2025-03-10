/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsen <bsen@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:55:19 by bsen              #+#    #+#             */
/*   Updated: 2024/09/05 13:23:48 by bsen             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	one_philo(t_philo *philo)
{
	print_status(philo, "take a fork");
	sensitive_usleep(philo->data->time_to_die);
}

int	mutex_free(t_data *data, int x, int y)
{
	int	i;

	i = -1;
	while (x >= 1 && ++i < y)
		pthread_mutex_destroy(&data->forks[i]);
	if (x >= 2)
		pthread_mutex_destroy(&data->anyone_dead_m);
	if (x > 3)
		pthread_mutex_destroy(&data->print_m);
	if (x > 4)
		pthread_mutex_destroy(&data->time_t);
	if (x > 5)
		pthread_mutex_destroy(&data->had_enough_m);
	if (x >= 1)
		free(data->forks);
	return (1);
}
