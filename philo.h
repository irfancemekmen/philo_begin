#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define THINKING 0
# define SLEEP 1
# define EATING 2

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					philo_id;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	int philo_num;               // filo sayısı
	int die_time;                //ölme süresi
	int eat_time;                // yeme süresi
	int sleep_time;              // uyku süresi
	int must_eat;                // yemesi gereken yemek sayısı
	int *meals_eaten;            // yediği yemek sayısı
	int sim_stop;                // simülasyonun durma yeri
	int *states;                 // filo durumları
	long long *last_meal_time;   // son yemek yeme zamanı
	long long sim_start;         // simülasyonun başlangıç zamanı
	pthread_t *threads;          // threadler
	pthread_t death_monitor;     //ölüm kontrolü
	pthread_mutex_t *forks;      //çatallar
	pthread_mutex_t state_mutex; // durum değişimi için mutex
	pthread_cond_t *can_eat;     // yemek yeme koşulu
	t_philo *philos;             // philolara erişim
}						t_data;

// philo.c
int						philo_creat(t_data *data, t_philo *philo);
int						death_thread_creat(t_data *data);
//init.c
int						init_sim(t_data *data);
int						av_init(t_data *data, char **av);
//time.c
long long				get_current_time(void);
void					ft_sleep(long long ms);
// free.c
void					free_func(t_data *data, int flag);
void					destroy_clean(t_data *data);
int						return_func(t_data *data, t_philo *philo, int flag);

void					cleanup(t_data *data, t_philo *philos);
void					*death_monitor_func(void *arg);
void					*philosopher(void *arg);
void					put_forks(t_philo *philo);
void					take_forks(t_philo *philo);
void					check_philosophers(t_data *data, int id);

#endif
