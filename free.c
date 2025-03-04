#include "philo.h"

void	free_func(t_data *data, int flag)
{
	if (flag > 0)
		free(data->threads);
	if (flag > 1)
		free(data->forks);
	if (flag > 2)
		free(data->can_eat);
	if (flag > 3)
		free(data->states);
	if (flag > 4)
		free(data->meals_eaten);
	if (flag > 5)
		free(data->last_meal_time);
}
