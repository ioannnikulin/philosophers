CC = cc
NAME = philo
PREFIX = 
COMPILE_FLAGS = -Wall -Wextra -Werror -pthread
LINK_FLAGS = -pthread
INCLUDES = -I.
MEMCHECK = valgrind --leak-check=full

all: $(NAME)

SRCS = init.c philo.c report.c setup.c utils.c main.c finalize.c wrappers.c t_s_ull.c t_s_usec.c monitor.c time.c print.c state_description.c philo_eating.c #t_s_int.c 
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

test1_cmd = ./$(NAME) 1 800 200 200

test1:
	#shouldn't eat a single time and dies
	eval $(test1_cmd)
	#didn't eat a single time and died

debug1:
	gdbtui --args $(test1_cmd)

test2_cmd = ./$(NAME) 5 800 200 200

test2:
	#everybody lives
	eval $(test2_cmd)
	#everybody lived until you killed them

debug2:
	gdbtui --args $(test2_cmd)

test3_cmd = ./$(NAME) 5 800 200 200 7

test3:
	#everybody lives until they've eaten 7 times
	eval $(test3_cmd)
	#everybody got full

memcheck:
	$(PREFIX)$(MEMCHECK) ./$(NAME) 2 800 200 200 1

test4:
	#everybody lives
	./$(NAME) 4 410 200 200
	#everybody lived until you killed them

test5_cmd = ./$(NAME) 3 310 200 100

test5:
	#one dies
	eval $(test5_cmd)
	#one died

memcheck5:
	$(PREFIX)$(MEMCHECK) $(test5_cmd)

test6_cmd = ./$(NAME) 2 800 200 100

debug6:
	gdbtui --args $(test6_cmd)

test6:
	#both live
	eval $(test6_cmd)
	#both lived until you killed them

PHONY: all clean fclean re
