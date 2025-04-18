#include "philo.h"

int	try_take_forks(t_philo *philo)
{
	t_data	*data;
	int		left_fork;
	int		right_fork;

	data = philo->data;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % data->philosopher_count;
	// Tek filozoflu durum için özel kontrol
	if (data->philosopher_count == 1)
	{
		one_philosopher(data, philo, left_fork);
		return (0);
	}
	// Çift ve tek filozoflar için ayrı çatal alma stratejileri
	// Çift numaralı filozoflar önce sol, sonra sağ çatal alır
	// Tek numaralı filozoflar önce sağ, sonra sol çatal alır
	if (philo->id % 2 == 0)
	{
		if (single_philo(philo, data, left_fork, right_fork) == 0)
			return (0);
	}
	else
	{
		if (double_philo(philo, data, left_fork, right_fork) == 0)
			return (0);
	}
	return (1);
}
int	single_philo(t_philo *philo, t_data *data, int lfork, int rfork)
{
	if (pthread_mutex_lock(&data->forks[lfork]) != 0)
		return (0);
	safe_print(data, philo->id, "has taken a fork", YELLOW);
	if (pthread_mutex_lock(&data->forks[rfork]) != 0)
	{
		pthread_mutex_unlock(&data->forks[lfork]);
		return (0);
	}
	safe_print(data, philo->id, "has taken a fork", YELLOW);
	return (1);
}
int	double_philo(t_philo *philo, t_data *data, int lfork, int rfork)
{
	if (pthread_mutex_lock(&data->forks[rfork]) != 0)
		return (0);
	safe_print(data, philo->id, "has taken a fork", YELLOW);
	if (pthread_mutex_lock(&data->forks[lfork]) != 0)
	{
		pthread_mutex_unlock(&data->forks[rfork]);
		return (0);
	}
	safe_print(data, philo->id, "has taken a fork", YELLOW);
	return (1);
}

// Daha dengeli bir filozof davranışı için take_forks fonksiyonu
void	take_forks(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	// Çatalları almak için deneme yapıyoruz
	while (!check_simulation_stop(data))
	{
		if (try_take_forks(philo))
		{
			// Çatallar alındı, yemek yeme durumuna geçiş
			pthread_mutex_lock(&data->state_mutex);
			// Yemek zamanını güncellemek kritik - hemen güncelle
			data->last_meal_time[philo->id] = get_current_time_ms();
			pthread_mutex_unlock(&data->state_mutex);
			return ;
		}
	}
}

void	put_forks(t_philo *philo)
{
	t_data	*data;
	int		left_fork;
	int		right_fork;

	data = philo->data;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % data->philosopher_count;
	// Mutex'leri bırakma
	pthread_mutex_unlock(&data->forks[left_fork]);
	pthread_mutex_unlock(&data->forks[right_fork]);
	// Durum güncelleme
	pthread_mutex_lock(&data->state_mutex);
	data->states[philo->id] = THINKING;
	pthread_mutex_unlock(&data->state_mutex);
}
