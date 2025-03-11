/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iekmen <iekmen@student.42kocaeli.com.tr>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:56:19 by bsen              #+#    #+#             */
/*   Updated: 2025/03/11 01:05:20 by iekmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdint.h>

typedef struct s_data
{
	int				nb_philo;
	int				nb_eat;
	bool			dead;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	int				nb_must_eat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	anyone_dead_m;
	pthread_mutex_t	print_m;
	uint64_t		start_time;
	pthread_mutex_t	last_meal_m;
	pthread_mutex_t	time_t;
	pthread_mutex_t	had_enough_m;
}					t_data;

typedef struct s_phlio
{
	int				id;
	int				had_enough;
	uint64_t		last_time;
	t_data			*data;
	int				meal_eaten;
	pthread_t		thread;

}					t_philo;

void				philos_begining(t_philo *philo);
void				*philo_life(void *p);
void				print_status(t_philo *philo, char *status);
int					ft_atoi(const char *str);
int					get_time(void);
void				sensitive_usleep(time_t time);
void				clean_data(t_data *data, t_philo *philo);
void				fork_and_eat(t_philo *philo);
void				sleep_and_think(t_philo *philo);
int					dead_or_alive(t_philo *philo);
int					deadcheck(t_philo *philo);
void				last_t_control(t_philo *philo);
void				whodied(t_philo *philo);
int					m_init(t_data *data);
int					mutex_free(t_data *data, int x, int y);
void				one_philo(t_philo *philo);
void				thread_join(t_philo *philos, int len);

#endif
