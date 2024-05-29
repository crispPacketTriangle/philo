#include "philo.h"

static int	conv(char digits[], int n);
static int	l_zeros(const char *nptr, int i);
static int	appnd_digits(const char *nptr, int i, int sign);

int	ph_atoi(const char *nptr)
{
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	i = l_zeros(nptr, i);
	return (appnd_digits(nptr, i, sign));
}

static int	conv(char digits[], int n)
{
	int	sum;
	int	pwr;

	n -= 1;
	sum = 0;
	pwr = 1;
	while (n > -1)
	{
		sum = sum + ((digits[n] - '0') * pwr);
		pwr *= 10;
		n--;
	}
	return (sum);
}

static int	l_zeros(const char *nptr, int i)
{
	while (nptr[i] == '0')
		i++;
	return (i);
}

static int	appnd_digits(const char *nptr, int i, int sign)
{
	int		n;
	char	digits[32];

	n = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digits[n] = nptr[i];
		i++;
		n++;
	}
	return (conv(digits, n) * sign); 
}
