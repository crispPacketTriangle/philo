#include "philo.h"

int	set_table(t_data *data)
{
	int	i;

	data->table = malloc(data->n_phil * sizeof(int));
	if (!data->table)
		return (1);
	i = 0;
	while (i < data->n_phil)
	{
		data->table[i] = -1;
		i++;
	}
	return (0);
}

int	init_locks(t_data *data)
{
	int	i;

	data->locks = malloc(data->n_phil * sizeof(pthread_mutex_t));
	if (!data->locks)
		return (1);
	i = 0;
	while (i < data->n_phil)
	{
		if (pthread_mutex_init(&data->locks[i], NULL) != 0)
		{
        	printf("Mutex init failed\n");
       		return (1);
		}
		i++;
	}
	return (0);
}

void	backoff(t_proc *p)
{
	if (p->state == 0)
		usleep(p->bkof * 100);
	if (p->state == 1)
	{
		p->bkof = 1;
		return ;
	}
	p->bkof *= 2;
}

void	release(t_proc *p)
{
	if (p->forks == 2 || p->forks == 3)
	{
		if (p->id > 0)
		{
			pthread_mutex_lock(&(p->locks[p->id - 1]));
			p->tb[p->id - 1] = -1;
			pthread_mutex_unlock(&(p->locks[p->id - 1]));
		}
		if (p->id == 0)
		{
			pthread_mutex_lock(&(p->locks[p->n - 1]));
			p->tb[p->n - 1] = 1;
			pthread_mutex_unlock(&(p->locks[p->n - 1]));
		}
	}
	if (p->forks == 1 || p->forks == 3)
	{
		pthread_mutex_lock(&(p->locks[p->id]));
		p->tb[p->id] = -1;
		pthread_mutex_unlock(&(p->locks[p->id]));
	}
}


// void	free_thrds()
//
// void	free_locks()







