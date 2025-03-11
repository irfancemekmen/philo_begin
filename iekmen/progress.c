#include "philo.h"

void	*philosopher(void *av)
{
	t_philo *philo;
	t_data *data;
	int id;
	int all_ate_enough;
	int i;

	philo = (t_philo *)av;
	data = philo->data;
	id = philo->philo_id;
	if (id % 2 == 0)
		ft_sleep(10);
	while (!data->sim_stop)
	{
		printf("%lld ms: %d is thinking.\n", current_time() - data->sim_start, philo->philo_id + 1);
		ft_sleep(data->sleep_time);
		if (data->sim_stop)
			break;
		take_forks(philo);
		if (data->sim_stop)
			break;
		ft_sleep(data->eat_time);
		pthread_mutex_lock(&data->state_mutex);
		data->meals_eaten[id]++;
		if (data->must_eat > 0 && data->meals_eaten[id] >= data->must_eat)
		{
			all_ate_enough = 1;
			i = -1;
			while (++i < data->philo_num)
			{
				if (data->meals_eaten[i] < data->must_eat)
				{
					all_ate_enough = 0;
					break;
				}
			}
			if (all_ate_enough)
			{
				data->sim_stop = 1;
				printf("%lld ms: Tüm filolar doydu.\n", current_time() - data->sim_start);
				pthread_mutex_unlock(&data->state_mutex);
				break;
			}
		}
		pthread_mutex_unlock(&data->state_mutex);
		put_forks(philo);
	}
	return (NULL);
}
void *death_monitor_func(void *arg)
{
    t_data *data = (t_data *)arg;
    long long cur_time;

    // Ölüm kontrolünden önce biraz bekleyelim
    ft_sleep(data->die_time / 2);

    while (!data->sim_stop)
    {
        pthread_mutex_lock(&data->state_mutex);
        cur_time = current_time();

        for (int i = 0; i < data->philo_num; i++)
        {
            // Eğer filozof yemek yemiyorsa ve son yemek yediği zamandan beri time_to_die ms geçtiyse
            if (cur_time - data->last_meal_time[i] > data->die_time)
            {
                printf("%lld ms: %d numaralı filozof açlıktan öldü! Son yemek: %lld ms önce\n" ,
                       cur_time - data->sim_start, i + 1,
                       cur_time - data->last_meal_time[i]);
                data->sim_stop = 1;
                pthread_mutex_unlock(&data->state_mutex);
                return NULL;
            }
        }

        pthread_mutex_unlock(&data->state_mutex);
        usleep(1000); // 1ms bekleyerek CPU kullanımını azalt
    }

    return NULL;
}

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

void put_forks(t_philo *philo)
{
    t_data *data = philo->data;
    int id = philo->philo_id;

    pthread_mutex_lock(&data->state_mutex);

    // Yeme iznini kaldır
    data->eat_flags[id] = 0;

    data->states[id] = THINKING;
    printf("%lld ms: %d numaralı filozof düşünüyor.\n",
           current_time() - data->sim_start, id);

    // Sol ve sağdaki filozofların durumlarını kontrol et
    check_philo(data, (id + data->philo_num - 1) % data->philo_num);
    check_philo(data, id % data->philo_num);

    pthread_mutex_unlock(&data->state_mutex);
}