#ifndef PHILO_H
#define PHILO_H


#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

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
	int				te;
	int				ts;
	int				td;
	int				id;
	int				bkof;
	int				state;
	int				forks;
	long long		lst_ate;
	pthread_t		thr;
	pthread_mutex_t *lock;
	pthread_mutex_t	*locks;
	int				*tb;
	int				lst_wrds;
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
	pthread_mutex_t *locks;
	int				*table;
}	t_data;

int			ph_atoi(const char *nptr);
void		*live(void *data);
int			eat(t_proc *pdata);
void		release(t_proc *p);
int			snooze(t_proc *pdata);
int			think(t_proc *pdata);
int			dead(t_proc *p);
int			init_vars(t_data *data, char **argv);
int			init_locks(t_data *pdata);
int			set_table(t_data *data);
int			init_threads(t_data *data, char **argv);
int			is_lpyr(int year);
int			days_in_m(int month, int year);
void		cal_year(t_data *data);
void		cal_month(t_data *data);
void		cal_sec(t_data *data);
void		backoff(t_proc *p);
long long 	time_ml();

#endif
