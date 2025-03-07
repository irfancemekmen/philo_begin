#include "philo.h"

void	check_philo(t_data *data, int id)
{
	int	left;
	int	right;

	left = (id + data->philo_num - 1) % data->philo_num;
	right = (id + 1) % data->philo_num;
	if (data->states[id] == SLEEP && data->states[left] != EATING
		&& data->states[right] != EATING)
	{
		data->states[id] = EATING;
		data->last_meal_time[id] = current_time();
		data->eat_flags[id] = 1;
	}
}

void	take_forks(t_philo *philo)
{
	t_data	*data;
	int		can_eat;

	data = philo->data;
	pthread_mutex_lock(&data->state_mutex);
	printf("%lld ms: %d numaralo filo uyuyor.\n", current_time()
		- data->sim_start, philo->philo_id + 1);
	check_philo(data, philo->philo_id);
	can_eat = data->eat_flags[philo->philo_id];
	pthread_mutex_unlock(&data->state_mutex);
	if (!can_eat)
	{
		while (!data->sim_stop)
		{
			usleep(100);
			pthread_mutex_lock(&data->state_mutex);
			can_eat = data->eat_flags[philo->philo_id];
			pthread_mutex_unlock(&data->state_mutex);
			if (can_eat || data->sim_stop)
				break ;
		}
	}
	if (!data->sim_stop)
	{
		pthread_mutex_lock(&data->state_mutex);
		printf("%lld ms: %d numaralı filo yemek yiyor.\n", current_time()
			- data->sim_start, philo->philo_id + 1);
		pthread_mutex_unlock(&data->state_mutex);
	}
}
