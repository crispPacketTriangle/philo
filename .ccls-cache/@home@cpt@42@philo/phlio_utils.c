#include "philo.h"

int	set_table(t_data *data)
{
	data->table = malloc(data->n_phil * sizeof(int));
	if (!data->table)
		return (1);
	return (0);
}

