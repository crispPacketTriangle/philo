#include "philo.h"

// stdbuf -o0 ./philo 5 2 1000 1000 | tee fileout

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
	//pthread_detach(thrd2);
	
	init_threads(&data, argv);

	i = 0;
	while (i < data.n_phil)
	{
		pthread_join(data.thrds[i]->thr, NULL);
		i++;
	}
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

	data->thrds = malloc ((data->n_phil + 1) * sizeof(t_proc *));
	i = 0;
	while (i < data->n_phil)
	{
		data->thrds[i] = malloc(sizeof(t_proc));
		i++;
	}
	i = 0;
	while (i < data->n_phil)
	{
		if (i % 2 == 0)
			data->thrds[i]->state = 0;
		if (i % 2 != 0)
			data->thrds[i]->state = 0;
		data->thrds[i]->n = data->n_phil;
		data->thrds[i]->te = data->t_to_eat;
		data->thrds[i]->ts = data->t_to_sleep;
		data->thrds[i]->lock = &data->lock;
		data->thrds[i]->id = i;
		data->thrds[i]->tb = data->table;
		pthread_create(&(data->thrds[i]->thr), NULL, &live, data->thrds[i]);
		i++;
	}
	data->thrds[i] = NULL;
	return (0);
}

void	*live(void *pdata)
{
	// here should be a continuous loop of eat,
	// sleep, think
	while (1)
	{
	t_proc	*p = (t_proc *)pdata;	

	// since i am checking the state of both forks in the following condition
	// does that mean they are both garunteed to be locked
	// or do i need to lock check lock
	pthread_mutex_lock(p->lock);
	if (p->state == 0 && p->id > 0 && p->tb[p->id - 1] == 0 && p->tb[p->id] == 0)
	{
		p->tb[p->id] = 1;
		p->tb[p->id - 1] = 1;
		p->forks = 1;
	}
	if (p->state == 0 && p->id == 0 && p->tb[p->n - 1] == 0 && p->tb[p->id] == 0)
	{
		p->tb[p->id] = 1;
		p->tb[p->n - 1] = 1;
		p->forks = 1;
	}
	pthread_mutex_unlock(p->lock);
	if (p->forks)
		eat(p);
	if (p->state == 1)
		snooze(p);
	// if (p->state == 2)
	// 	think(p);
	}
	return (0);
}

int	eat(t_proc *pdata)
{
	t_proc	*p = (t_proc *)pdata;
	usleep(p->te);
	pthread_mutex_lock(p->lock);
	printf("%lld :", time_ml());
	printf("hi from process %d, forks: %d\n", p->id, p->tb[p->id]);
	if (p->id == 0)
	{
		p->tb[p->id] = 0;
		p->tb[p->n - 1] = 0;
	}
	else
	{
		p->tb[p->id] = 0;
		p->tb[p->id - 1] = 0;
	}
	pthread_mutex_unlock(p->lock);
	p->forks = 0;
	p->state = 1;
	return (0);
}	

int	snooze(t_proc *pdata)
{
	t_proc	*p = (t_proc *)pdata;
	p->state = 0;
	usleep(p->ts);
	return (0);
}

// int	think(t_proc *pdata)
// {
// 	t_proc	*p = (t_proc *)pdata;
// 	p->state = 0;
// 	return (0);
// }


// a way to track if a thread has eaten or not
// 0 = hungry
// 1 = tired
// 2 = alert
//
// if 0 eat
// if 1 sleep
// if 2 think
//



