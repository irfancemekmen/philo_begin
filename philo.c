#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo	*philos;
	t_data	*data;
	int		id;

	philos = (t_philo *)arg;
	data = philos->data;
	id = philos->philo_id;
	if (id % 2 == 0)
		ft_sleep(10);
	while(!data->sim_stop)
	{
		printf("%lld ms: %d is thinking.\n", get_current_time() - data->sim_start, id);
		ft_sleep(data->sleep_time);
		if (data->sim_stop)
			break;
	}

}

int	main(int ac, char **av)
{
	t_data	*data;
	t_philo	*philos;
	int		i;
	int		j;

	if (ac < 5 || ac > 6)
	{
		printf("Check Arguments!\n");
		return (0);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (0);
	if (av_init(data, av))
	{
		printf("Error!\n");
		free(data);
		return (0);
	}
	if (init_sim(data))
	{
		cleanup(data, NULL);
		printf("Simülasyon başlatılamadı!");
		return (0);
	}
	philos = malloc(sizeof(t_philo) * data->philo_num);
	if (!philos)
	{
		cleanup(data, NULL);
		return (1);
	}
	i = 0;
	while (i < data->philo_num)
	{
		philos[i].philo_id = i + 1;
		philos[i].data = data;
		if (pthread_create(&data->threads[i], NULL, philosopher,
				&philos[i]) != 0)
		{
			data->sim_stop = 1;
			j = 0;
			while (j < i)
			{
				pthread_join(data->threads[j], NULL);
				j++;
			}
			cleanup(data, philos);
			printf("thread oluşturulamadı!\n");
			return (0);
		}
		i++;
	}
	free(data);
	return (0);
}
