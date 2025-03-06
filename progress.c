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
		data->last_meal_time[id] = get_current_time();
		pthread_cond_signal(&data->can_eat[id]);
	}
}
