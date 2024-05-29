#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

# define SEC_IN_MIN 60
# define SEC_IN_HOUR 3600
# define SEC_IN_DAY 86400
# define DAYS_IN_YR 365
# define DAYS_IN_LPYR 366

typedef struct t_time
{
	int 	year;
	int		month;
	int 	days;
	int		hour;
	int		minute;
	time_t	sec;
	int		r_sec;
	int		second;
	int 	msecond;
}	t_time;

// state:
//  0: hungry
//  1: tired
//  2: alert
typedef struct t_proc
{
	int				n;
	int				id;
	int				state;
	int				forks;
	pthread_t		thr;
	pthread_mutex_t	*lock;
	int				*tb;
}	t_proc;

typedef struct t_data
{
	int				n_phil;
	int				t_to_die;
	int				t_to_eat;
	int				t_to_sleep;
	long long		n;
	t_time			t;
	t_proc			**thrds;
	pthread_mutex_t	lock;
	int				*table;
}	t_data;

int			ph_atoi(const char *nptr);
void		*live(void *data);
int			eat(t_proc *pdata);
int			snooze(t_proc *pdata);
int			init_vars(t_data *data, char **argv);
int			set_table(t_data *data);
int			init_threads(t_data *data, char **argv);
int			is_lpyr(int year);
int			days_in_m(int month, int year);
void		cal_year(t_data *data);
void		cal_month(t_data *data);
void		cal_sec(t_data *data);
long long 	time_ml();

#endif
