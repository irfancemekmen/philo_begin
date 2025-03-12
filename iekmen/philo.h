#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// eat>sleep>think sıralaması
# define THINKING 0
# define SLEEP 1
# define EATING 2

typedef struct s_data
{
	int philo_num;                    // filo sayısı
	int fork_num;                     // çatal sayısı
	int dead;                         // ölü kontrolü
	int die_time;                     //ölme süresi
	int eat_time;                     // yeme süresi
	int sleep_time;                   // uyku süresi
	int must_eat;                     // yemesi gereken yemek sayısı
	int start_time;                   // başlangıç zamanı
	pthread_mutex_t *forks;           //çatallar
	pthread_mutex_t anyone_die;       // biri öldü mü
	pthread_mutex_t print_mutex;      //
	pthread_mutex_t last_meal_mutex;  //
	pthread_mutex_t time_t;           //
	pthread_mutex_t had_enough_mutex; //
	pthread_t ctrl;                   //
}				t_data;

typedef struct s_philo
{
	int			philo_id;
	int			had_enough;
	int			last_time;
	int			meal_eaten;
	pthread_t	thread;
	t_data		*data;
}				t_philo;

// philo.c
int				philo_creat(t_data *data, t_philo *philo);
int				death_thread_creat(t_data *data);
// init.c
int				init_sim(t_data *data);
int				av_init(t_data *data, char **av);
void			mutex_init_func(t_data *data);
// time.c
long long		current_time(void);
void			ft_sleep(long long ms);
// free.c
int				return_func(t_data *data, t_philo *philo, int flag);
int				mutex_dest(t_data *data, int flag, int x);
// progress.c
void			check_philo(t_data *data, int id);
// utils.c
int				ft_atoi(const char *str);

void			cleanup(t_data *data, t_philo *philos);
void			*death_monitor_func(void *arg);
void			*philosopher(void *arg);
void			put_forks(t_philo *philo);
void			take_forks(t_philo *philo);
void			check_philosophers(t_data *data, int id);

#endif
