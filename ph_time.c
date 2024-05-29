#include "philo.h"

int	is_lpyr(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
			return (1);
	return (0);
}

int	days_in_m(int month, int year)
{
	int	days_p_m[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 1 && is_lpyr(year))
			return (29);
	return (days_p_m[month]);
}

//only call once in init
void	cal_year(t_data *data)
{
	int	days_in_yr;
	struct timeval 	tv;
	
	gettimeofday(&tv, NULL);
	data->t.sec = tv.tv_sec;
	suseconds_t micro_sec = tv.tv_usec;
	data->t.year = 1970;
	data->t.days = (data->t.sec / SEC_IN_DAY) + 1;
	while (1)
	{
		if (is_lpyr(data->t.year))
			days_in_yr = DAYS_IN_LPYR;
		else
			days_in_yr = DAYS_IN_YR;
		if (data->t.days >= days_in_yr)
		{
			data->t.days -= days_in_yr;
			data->t.year++;
		}
		else
			break;
	}
}

// only call once in init
void	cal_month(t_data *data)
{
	int	days_i_m;

	data->t.month = 1;
	while (1)
	{
		days_i_m = days_in_m(data->t.month, data->t.year);
		if (data->t.days >= days_i_m)
		{
			data->t.days -= days_i_m;
			data->t.month++;
		}
		else
			break;
	}
}

void	cal_sec(t_data *data)
{
	data->t.r_sec = data->t.sec % SEC_IN_DAY;
	data->t.hour = (data->t.r_sec / SEC_IN_HOUR) + 2;
	data->t.r_sec %= SEC_IN_HOUR;
	data->t.minute = data->t.r_sec / SEC_IN_MIN;
	data->t.second = data->t.r_sec % SEC_IN_MIN;
}

long long time_ml()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
}

