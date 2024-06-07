#include "philo.h"

// stdbuf -o0 ./philo 5 1000 1000 1000 | tee fileout
// valgrind --tool=helgrind -s ./philo 4 310 200 100 | tee fileout 

// pthread_mutex_t	lock;
// pthread_t 		thrd1;
// pthread_t 		thrd2;

// Your(s) program(s) should take the following arguments:
// number_of_philosophers time_to_die time_to_eat time_to_sleep
// [number_of_times_each_philosopher_must_eat]

// timestamp_in_ms X has taken a fork
// timestamp_in_ms X is eating
// timestamp_in_ms X is sleeping
// timestamp_in_ms X is thinking
// timestamp_in_ms X died


// check inputs as valid, n inputs, no chars, etc

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if(init_vars(&data, argv) > 0)
		return (1);
	cal_year(&data);
	cal_month(&data);
	cal_sec(&data);
	printf("%d.%d.%d\n%d:%d:%d\n",
			data.t.days, data.t.month, data.t.year, 
			data.t.hour, data.t.minute, data.t.second);

	if (pthread_mutex_init(&data.lock, NULL) != 0)
	{
        printf("Mutex init failed\n");
        return (1);
	}
	init_locks(&data);	
	init_threads(&data, argv);
	
	int ofs = 1000;
	i = 0;
	while (i < data.n_phil)
	{
		usleep(ofs);
		pthread_join(data.thrds[i]->thr, NULL);
		i++;
		ofs += 1500;
	}
	// need another thread to check if any threads have died
}

int	init_vars(t_data *data, char **argv)
{
	data->n_phil = ph_atoi(argv[1]);
	data->t_to_die = ph_atoi(argv[2]);
	data->t_to_eat = ph_atoi(argv[3]);
	data->t_to_sleep = ph_atoi(argv[4]);
	if (set_table(data) > 0)
		return (1);
	return (0);
}

int	init_threads(t_data *data, char **argv)
{
	int	i;

	data->thrds = malloc ((data->n_phil) * sizeof(t_proc *));
	i = 0;
	while (i < data->n_phil)
	{
		data->thrds[i] = malloc(sizeof(t_proc));
		i++;
	}
	i = 0;
	while (i < data->n_phil)
	{
		// if (i % 2 == 0)
		// 	data->thrds[i]->state = 1;
		// if (i % 2 != 0)
		data->thrds[i]->state = 1;
		data->thrds[i]->forks = 0;
		data->thrds[i]->n = data->n_phil;
		data->thrds[i]->te = data->t_to_eat;
		data->thrds[i]->ts = data->t_to_sleep;
		data->thrds[i]->td = data->t_to_die;
		data->thrds[i]->bkof = 1;
		data->thrds[i]->lst_ate = time_ml();
		data->thrds[i]->lock = &data->lock;
		data->thrds[i]->locks = data->locks;
		data->thrds[i]->id = i;
		data->thrds[i]->tb = data->table;
		data->thrds[i]->lst_wrds = 0;
		pthread_create(&(data->thrds[i]->thr), NULL, &live, data->thrds[i]);
		i++;
	}
	return (0);
}

void	*live(void *pdata)
{
	t_proc	*p = (t_proc *)pdata;	
	while (1)
	{

	// top condition should be after first fork grab
	if (p->n > 1)
	{
			dead(p);
			if (p->state == 0 && (p->forks == 0 || p->forks == 1))
			{
				if (p->id > 0)
				{
					pthread_mutex_lock(&(p->locks[p->id - 1]));
					if (p->tb[p->id - 1] == -1)
					{
						p->tb[p->id - 1] = 1;
						p->forks += 2;
					}
					pthread_mutex_unlock(&(p->locks[p->id - 1]));
				}
				if (p->id == 0)
				{
					pthread_mutex_lock(&(p->locks[p->n - 1]));
					if (p->state == 0 && p->tb[p->n - 1] == -1)
					{
						p->tb[p->n - 1] = 1;
						p->forks += 2;
					}
					pthread_mutex_unlock(&(p->locks[p->n - 1]));
				}
			}
			dead(p);
			if (p->state == 0 && (p->forks == 0 || p->forks == 2))
			{
				pthread_mutex_lock(&(p->locks[p->id]));
				if (p->tb[p->id] == -1)
				{
					p->tb[p->id] = 1;
					p->forks++;
				}	
				pthread_mutex_unlock(&(p->locks[p->id]));
			}
		}

////////////////////////////////////////////////////////

		dead(p);
		if (p->forks == 3 && p->state != 2)
			eat(p);
		if (p->state == 1)
			snooze(p);
		if (p->state == 2)
		{
			release(p);
			pthread_mutex_lock(&(p->locks[p->id]));
			printf("%d died (last meal %lld ms ago)\n", p->id, time_ml() - p->lst_ate);
			pthread_mutex_unlock(&(p->locks[p->id]));
			return(NULL);
		}
		//backoff(p);
	}
	return (0);
}

int	eat(t_proc *p)
{
	//printf("%d - %lld\n", p->id, time_ml() - p->lst_ate);
	p->lst_ate = time_ml();
	usleep(p->te * 1000);
	pthread_mutex_lock(&(p->locks[p->id]));
	printf("%d: %lld\n", p->id, time_ml());
	pthread_mutex_unlock(&(p->locks[p->id]));
	if (p->id == 0)
	{
		pthread_mutex_lock(&(p->locks[p->id]));
		p->tb[p->id] = -1;
		pthread_mutex_unlock(&(p->locks[p->id]));
		pthread_mutex_lock(&(p->locks[p->n - 1]));
		p->tb[p->n - 1] = -1;
		pthread_mutex_unlock(&(p->locks[p->n - 1]));
	}
	else
	{
		pthread_mutex_lock(&(p->locks[p->id]));
		p->tb[p->id] = -1;
		pthread_mutex_unlock(&(p->locks[p->id]));
		pthread_mutex_lock(&(p->locks[p->id - 1]));
		p->tb[p->id - 1] = -1;
		pthread_mutex_unlock(&(p->locks[p->id - 1]));
	}
	p->forks = 0;
	p->state = 1;
	return (0);
}	

int	snooze(t_proc *p)
{
	p->state = 0;
	usleep(p->ts * 1000);
	return (0);
}

int	dead(t_proc *p)
{
	if (time_ml() - p->lst_ate >= p->td)
	{
		p->state = 2;
		return (1);
	}
	return (0);
}

