#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define THINKING 0
# define EATING 1
# define SLEEP 2

# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define RESET "\x1B[0m"

typedef struct s_data
{
	int philosopher_count;       // Filozof sayısı
	int must_eat_count;          // yemesi gereken minimum yemek sayısı
	int *meals_eaten;            // Her filozofun yediği yemek sayısı
	int *states;                 // Filozofların durumları
	int time_to_die;             // Açlıktan ölme süresi (ms)
	int time_to_eat;             // Yemek yeme süresi (ms)
	int time_to_sleep;           // Uyuma süresi (ms)
	int simulation_stop;         // Simülasyonu durdurma bayrağı
	long long simulation_start;  // Simülasyon başlangıç zamanı
	long long *last_meal_time;   // Her filozofun son yemek yeme zamanı
	pthread_t *threads;          // Thread'ler
	pthread_t death_monitor;     // Ölüm kontrolü için thread
	pthread_mutex_t state_mutex; // Durum değişiklikleri için mutex
	pthread_mutex_t *forks;      // Çatallar (mutex'ler)
	pthread_mutex_t stop_mutex;  // Simülasyonu durdurma bayrağı için mutex
	pthread_mutex_t print_mutex; // Yazdırma için mutex
}			t_data;

typedef struct s_philo
{
	int id;       // Filozof ID'si (0'dan başlar)
	t_data *data; // Program verileri
}			t_philo;

// forks_ctrl.c
int			try_take_forks(t_philo *philo);
int			single_philo(t_philo *philo, t_data *data, int lfork, int rfork);
int			double_philo(t_philo *philo, t_data *data, int lfork, int rfork);
void		take_forks(t_philo *philo);
void		put_forks(t_philo *philo);

// initialization.c
int			av_config(char **av, t_data *data);
int			mutex_initialization(t_data *data);
int			init_simulation(t_data *data);

// main.c
int			cleanup(t_data *data, t_philo *philos, int error, int flag);
int			init_destroy(t_data *data, int flag);

//philo_dead_ctrl.c
void		*death_monitor_func(void *arg);
int			check_philo_death(t_data *data, int i);
int			philo_dead_control(t_data *data);
int			thread_start(t_data *data, t_philo *philos);
int			thread_start_v2(t_data *data);
void		one_philosopher(t_data *data, t_philo *philo, int left_fork);

//philosopher.c
void		*philosopher(void *arg);
int			philosopher_v2(t_philo *philo, t_data *data, int id);
void		philo_enough_food(t_data *data);

//security.c
int			check_simulation_stop(t_data *data);
void		set_simulation_stop(t_data *data);
void		safe_print(t_data *data, int id, char *message, char *color);

//utils.c
long long	get_current_time_ms(void);
void		ft_sleep(long long ms);
int			str_contains(const char *str, const char *substr);
int			ft_atoi(char *str);

#endif
