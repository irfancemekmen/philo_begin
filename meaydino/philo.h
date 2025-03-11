#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#ifndef PHİLO_H
# define PHİLO_H
// Filozof durumları
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// Renkli çıktı için
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define RESET "\x1B[0m"

typedef struct s_data
{
    int philosopher_count;          // Filozof sayısı
    pthread_t *threads;             // Thread'ler
    pthread_t death_monitor;        // Ölüm kontrolü için thread
    pthread_mutex_t *forks;         // Çatallar (mutex'ler)
    pthread_mutex_t state_mutex;    // Durum değişiklikleri için mutex
    pthread_cond_t *can_eat;        // Yemek yiyebilme koşul değişkenleri
    int *states;                    // Filozofların durumları
    int *meals_eaten;               // Her filozofun yediği yemek sayısı
    long long *last_meal_time;      // Her filozofun son yemek yeme zamanı
    int time_to_die;                // Açlıktan ölme süresi (ms)
    int time_to_eat;                // Yemek yeme süresi (ms)
    int time_to_sleep;              // Uyuma süresi (ms)
    int must_eat_count;             // Her filozofun yemesi gereken minimum yemek sayısı
    int simulation_stop;            // Simülasyonu durdurma bayrağı
    long long simulation_start;     // Simülasyon başlangıç zamanı
} t_data;

typedef struct s_philo
{
    int id;                 // Filozof ID'si (0'dan başlar)
    t_data *data;           // Program verileri
} t_philo;

int philo_dead_control(t_data *data);
void *death_monitor_func(void *arg);
int thread_start(t_data *data);
void *philosopher(void *arg);
long long get_current_time_ms();
void mutex_initialization(t_data *data);
int free_func(int flag, t_data *data);
int init_simulation(t_data *data);
int av_config(char **av, t_data *data);
void ft_sleep(long long ms);
void check_philosopher(t_data *data, int id);
void put_forks(t_philo *philo);
void take_forks(t_philo *philo);
void philo_enough_food(t_data *data);
void cleanup(t_data *data, t_philo *philos);
int ft_atoi(char *str);






 #endif
