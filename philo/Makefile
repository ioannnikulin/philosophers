CC = cc
NAME = philo
PREFIX = 
COMPILE_FLAGS = -Wall -Wextra -Werror -pthread
LINK_FLAGS = -pthread
INCLUDES = -I.

all: $(NAME)

SRCS = init.c philo.c report.c setup.c utils.c main.c finalize.c wrappers.c t_s_int.c t_s_usec.c monitor.c
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(PREFIX)$(CC) $(LINK_FLAGS) $(OBJS) -o $(NAME)

$(OBJS): %.o: %.c
	$(PREFIX)$(CC) $(COMPILE_FLAGS) $< -o $@ -g $(INCLUDES) -c

clean:
	$(PREFIX)rm -f $(OBJS)

fclean: clean
	$(PREFIX)rm -f $(NAME)

re: fclean all

PHONY: all clean fclean re
