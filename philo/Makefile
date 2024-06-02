CC = cc
NAME = philo
PREFIX = 
COMPILE_FLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I.

all: $(NAME)

SRCS = init.c philo.c report.c setup.c utils.c main.c
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(PREFIX)$(CC) $(OBJS) -o $(NAME)

$(OBJS): %.o: %.c
	$(PREFIX)$(CC) $(COMPILE_FLAGS) $< -o $@ -g $(INCLUDES) -c

clean:
	$(PREFIX)rm -f $(OBJS)

fclean: clean
	$(PREFIX)rm -f $(NAME)

re: fclean all

PHONY: all clean fclean re
