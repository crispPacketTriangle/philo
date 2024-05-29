CC=cc -g
FLAGS=-Wall -Werror -Wextra
NAME=philo
LOGO=~/42/ascii/logo.sh
SRCS=philo.c ph_atoi.c ph_time.c philo_utils.c

OBJS= $(SRCS:.c=.o)
LIBFT=/libft/
LFT=libft.a

all: $(NAME)
		@if [ -f $(LOGO) ]; then \
			bash $(LOGO); \
		fi 

$(NAME): $(OBJS)
		$(CC) -o $(NAME) $(OBJS) $(FLAGS)

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re deps
