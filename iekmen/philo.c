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

	if (ac <5 || ac > 6)
		return(return_func(NULL, NULL, 1));
	data = malloc(sizeof(t_data));
	if (!data)
		return(return_func(NULL, NULL, 1));
	if (av_init(data, av))
		return (return_func(data,NULL, 1));
}
