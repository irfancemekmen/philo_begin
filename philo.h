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

typedef struct s_data
{
	int philo_num;               // filo sayısı
	int *eat_flags;              // yeme izni bayrakları
	int *states;                 // filo durumları
	int *meals_eaten;            // yediği yemek sayısı
	long long *last_meal_time;   // son yemek yeme zamanı
	int die_time;                //ölme süresi
	int eat_time;                // yeme süresi
	int sleep_time;              // uyku süresi
	int must_eat;                // yemesi gereken yemek sayısı
	int sim_stop;                // simülasyonun durma yeri
	long long sim_start;         // simülasyonun başlangıç zamanı
	pthread_t *threads;          // threadler
	pthread_t death_monitor;     //ölüm kontrolü
	pthread_mutex_t *forks;      //çatallar
	pthread_mutex_t state_mutex; // durum değişimi için mutex
	pthread_mutex_t *eat_mutex;  // yemek yeme koşulu
}			t_data;

typedef struct s_philo
{
	int		philo_id;
	t_data	*data;
}			t_philo;

// philo.c
int			philo_creat(t_data *data, t_philo *philo);
int			death_thread_creat(t_data *data);
// init.c
int			init_sim(t_data *data);
int			av_init(t_data *data, char **av);
// time.c
long long	current_time(void);
void		ft_sleep(long long ms);
// free.c
void		destroy_clean(t_data *data);
int			return_func(t_data *data, t_philo *philo, int flag);
// progress.c
void		check_philo(t_data *data, int id);

void		cleanup(t_data *data, t_philo *philos);
void		*death_monitor_func(void *arg);
void		*philosopher(void *arg);
void		put_forks(t_philo *philo);
void		take_forks(t_philo *philo);
void		check_philosophers(t_data *data, int id);

#endif
