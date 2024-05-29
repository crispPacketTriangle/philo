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
		data->table[i] = 0;
		i++;
	}
	return (0);
}

