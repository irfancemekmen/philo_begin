#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

int av_config(char **av, t_data *data)
{
    data->philosopher_count = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);
    data->time_to_eat = ft_atoi(av[3]);
    data->time_to_sleep = ft_atoi(av[4]);
    data->simulation_stop = 0;
    data->must_eat_count = -1;
    if(av[5])
        data->must_eat_count = ft_atoi(av[5]);
    if (data->philosopher_count <= 0 || data->time_to_die <= 0 ||
    data->time_to_eat <= 0 || data->time_to_sleep <= 0 ||
    (av[5] && data->must_eat_count <= 0))
    {
        printf("Geçersiz argümanlar. Tüm değerler pozitif olmalıdır.\n");
        return 1;
    }
    return 0;
}
int ft_atoi(char *str)
{
    int i;
    int sign;
    int result;

    i = 0;
    sign = 1;
    result = 0;
    while(str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
        i++;
    if(str[i] == '-' || str[i] == '+')
    {
        if(str[i] == '-')
            sign = -1;
        i++;
    }
    while(str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + str[i] - '0';
        i++;
    }
    return (result * sign);
}
int init_simulation(t_data *data)
{
    int ctrl;

    data->threads = malloc(sizeof(pthread_t) * data->philosopher_count);
    if (!data->threads)
        return 1;
    data->forks = malloc(sizeof(pthread_mutex_t) * data->philosopher_count);
    if (!data->forks)
        return 1;
    data->can_eat = malloc(sizeof(pthread_cond_t) * data->philosopher_count);
    if (!data->can_eat)
        return 1;
    data->states = malloc(sizeof(int) * data->philosopher_count);
    if (!data->states)
        return 1;
    data->meals_eaten = malloc(sizeof(int) * data->philosopher_count);
    if (!data->meals_eaten)
        return 1;
    data->last_meal_time = malloc(sizeof(long long) * data->philosopher_count);
    if (!data->last_meal_time)
        return 1;
    mutex_initialization(data);
    return 0;
}
void mutex_initialization(t_data *data)
{
    int i;

    i = -1;
    pthread_mutex_init(&data->state_mutex, NULL);
    data->simulation_start = get_current_time_ms();
    while(++i < data->philosopher_count)
    {
        pthread_mutex_init(&data->forks[i], NULL);
        pthread_cond_init(&data->can_eat[i], NULL);
        data->states[i] = THINKING;
        data->meals_eaten[i] = 0;
        data->last_meal_time[i] = data->simulation_start;
    }
}
// int can_philosopher_eat(t_data *data, int philosopher_id)
// {
//     // Mutex'i almak
//     pthread_mutex_lock(&data->phil_mutexes[philosopher_id]);

//     // Filozofun yemek yediği zamana göre kontroller
//     if (data->states[philosopher_id] == THINKING &&
//         data->forks_available(philosopher_id))
//     {
//         data->states[philosopher_id] = EATING;
//         data->meals_eaten[philosopher_id]++;
//         data->last_meal_time[philosopher_id] = get_current_time_ms();
//     }

//     // Mutex'i serbest bırakmak
//     pthread_mutex_unlock(&data->phil_mutexes[philosopher_id]);

//     return data->states[philosopher_id] == EATING;
// }
long long get_current_time_ms()
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}
void take_forks(t_philo *philo)
{
    t_data *data = philo->data;
    int id = philo->id;
    pthread_mutex_lock(&data->state_mutex);
    data->states[id] = HUNGRY;
    printf(YELLOW "%lld ms: %d numaralı filozof acıktı.\n" RESET,
           get_current_time_ms() - data->simulation_start, id + 1);
    // Filozofun durumunu kontrol et ve uygunsa yemesine izin ver
    check_philosopher(data, id);
    // Eğer yemek yiyemezse, sinyal gelene kadar bekle
    while (data->states[id] != EATING && !data->simulation_stop)
        pthread_cond_wait(&data->can_eat[id], &data->state_mutex);
    if (!data->simulation_stop)
        printf(GREEN "%lld ms: %d numaralı filozof yemek yiyor.\n" RESET,
               get_current_time_ms() - data->simulation_start, id + 1);
    pthread_mutex_unlock(&data->state_mutex);
}
void philo_enough_food(t_data *data)
{
    int all_ate_enough = 1;
    int i;
    i = -1;

    while(++i < data->philosopher_count)
    {
        if(data->meals_eaten[i] < data->must_eat_count)
        {
            all_ate_enough = 0;
            break;
        }
    }
    if(all_ate_enough)
    {
        data->simulation_stop = 1;
        printf(GREEN "%lld ms: Tüm filozoflar yeterince yemek yedi. Simülasyon sona eriyor.\n" RESET,get_current_time_ms() - data->simulation_start);
        pthread_mutex_unlock(&data->state_mutex);
    }
}
void check_philosopher(t_data *data, int id)
{
    int left = (id + data->philosopher_count - 1) % data->philosopher_count;
    int right = (id + 1) % data->philosopher_count;

    // Eğer filozof aç ve her iki çatal da müsaitse
    if (data->states[id] == HUNGRY &&
        data->states[left] != EATING &&
        data->states[right] != EATING)
    {
        data->states[id] = EATING;
        // Son yemek zamanını güncelle
        data->last_meal_time[id] = get_current_time_ms();
        pthread_cond_signal(&data->can_eat[id]);
    }
}
void put_forks(t_philo *philo)
{
    t_data *data = philo->data;
    int id = philo->id;

    pthread_mutex_lock(&data->state_mutex);

    data->states[id] = THINKING;
    printf(BLUE "%lld ms: %d numaralı filozof düşünüyor.\n" RESET,
           get_current_time_ms() - data->simulation_start, id + 1);

    // Sol ve sağdaki filozofların durumlarını kontrol et
    check_philosopher(data, (id + data->philosopher_count - 1) % data->philosopher_count);
    check_philosopher(data, (id + 1) % data->philosopher_count);

    pthread_mutex_unlock(&data->state_mutex);
}
void *philosopher(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;
    int id = philo->id;

    if(id % 2 == 0)
        ft_sleep(10);
    while(!data->simulation_stop)
    {
        printf(BLUE"%lld %d is thinking\n"RESET, get_current_time_ms() - data->simulation_start, id + 1);
        ft_sleep(data->time_to_sleep);
        if(data->simulation_stop)
            break;
        take_forks(philo);
        if(data->simulation_stop)
            break;
        ft_sleep(data->time_to_eat);
        pthread_mutex_lock(&data->state_mutex);
        data->meals_eaten[id]++;
        if (data->must_eat_count > 0 && data->meals_eaten[id] >= data->must_eat_count)
            philo_enough_food(data);
        pthread_mutex_unlock(&data->state_mutex);
        put_forks(philo);
    }
    return NULL;
}
void ft_sleep(long long ms)
{
    long long start_time = get_current_time_ms();
    long long current_time;

    while (1)
    {
        current_time = get_current_time_ms();
        if (current_time - start_time >= ms)
            break;
        usleep(100); // Daha kısa aralıklarla döngü, daha hassas zamanlama
    }
}
int thread_start(t_data *data)
{
    t_philo *philos;
    philos = malloc(sizeof(t_philo) * data->philosopher_count);
    if(!philos)
    {
        cleanup(data, NULL);
        return 1;
    }
    int i;
    int j;
    j = -1;
    i = -1;
    while(++i < data->philosopher_count)
    {
        philos[i].id = i;
        philos[i].data = data;
        if(pthread_create(&data->threads[i], NULL, philosopher, &philos[i]) != 0)
        {
            data->simulation_stop = 1;
            while(++j < i)
                pthread_join(data->threads[j], NULL);
            cleanup(data, philos);
            printf("thread oluşturulamadı");
            return(1);
        }
    }
    philo_dead_control(data);
    return 0;
}
void *death_monitor_func(void *arg)
{
    t_data *data = (t_data *)arg;
    long long current_time;
    // Ölüm kontrolünden önce biraz bekleyelim
    ft_sleep(data->time_to_die / 2);
    while (!data->simulation_stop)
    {
        pthread_mutex_lock(&data->state_mutex);
        current_time = get_current_time_ms();
        for (int i = 0; i < data->philosopher_count; i++)
        {
            // Eğer filozof yemek yemiyorsa ve son yemek yediği zamandan beri time_to_die ms geçtiyse
            if (current_time - data->last_meal_time[i] > data->time_to_die)
            {
                printf(RED "%lld ms: %d numaralı filozof açlıktan öldü! Son yemek: %lld ms önce\n" RESET,
                       current_time - data->simulation_start, i + 1,
                       current_time - data->last_meal_time[i]);
                data->simulation_stop = 1;
                pthread_mutex_unlock(&data->state_mutex);
                return NULL;
            }
        }
        pthread_mutex_unlock(&data->state_mutex);
        usleep(1000); // 1ms bekleyerek CPU kullanımını azalt
    }
    return NULL;
}
int philo_dead_control(t_data *data)
{
    t_philo *philos;
    int i;

    i = -1;
    if(pthread_create(&data->death_monitor, NULL, death_monitor_func, data) != 0)
    {
        data->simulation_stop = 1;
        while(++i < data->philosopher_count)
            pthread_join(data->threads[i], NULL);
        cleanup(data, philos);
        printf("Ölüm kontrolü thread'i oluşturulamadı\n");
        return 1;
    }
    while(++i < data->philosopher_count)
        pthread_join(data->threads[i], NULL);
    pthread_join(data->death_monitor, NULL);
    cleanup(data, philos);
    return 0;
}
void cleanup(t_data *data, t_philo *philos)
{
    int i;

    i = -1;
    if (data->threads)
    {
        free(data->threads);
        data->threads = NULL;
    }
    if (data->forks)
    {
        while(++i < data->philosopher_count)
            pthread_mutex_destroy(&data->forks[i]);
        free(data->forks);
    }
    if (data->can_eat)
    {
        while(++i < data->philosopher_count)
            pthread_cond_destroy(&data->can_eat[i]);
        free(data->can_eat);
    }
    pthread_mutex_destroy(&data->state_mutex);
    if (data->states)
        free(data->states);
    if (data->meals_eaten)
        free(data->meals_eaten);
    if (data->last_meal_time)
        free(data->last_meal_time);
    if (!philos)
        free(philos);
    free(data);
}
int main(int ac, char **av)
{
    t_data *data;
    if(ac < 5 || ac > 6)
    {
        printf("USAGE: %s <philosopher_number> <death_time> <eating_time> <sleeping_time> [number of meals each_philosopher_needs_to_eat]\n", av[0]);
        return (1);
    }
    data = malloc(sizeof(t_data));
    if(!data)
        return 1;
    if(av_config(av, data) || init_simulation(data))
    {
        cleanup(data, NULL);
        return 1;
    }
    thread_start(data);
    return (0);
}
