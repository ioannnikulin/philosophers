CC = cc
NAME = philo
PREFIX = 
COMPILE_FLAGS = -Wall -Wextra -Werror -pthread
LINK_FLAGS = -pthread
INCLUDES = -I.
MEMCHECK = valgrind --leak-check=full
DATARACE = valgrind --tool=helgrind 

all: $(NAME)

SRCS = init.c philo.c report.c setup.c utils.c main.c finalize.c wrappers.c t_s_ull.c t_s_ull_math.c t_s_usec.c monitor.c time.c print.c print_utils.c state_description.c philo_eating.c die_and_drop_forks.c philo_ret.c philo_smart_wait.c philo_forks.c t_s_ull_w.c atoui.c
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
	$(PREFIX)eval $(test1_cmd)
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
	$(PREFIX)eval $(test3_cmd)
	#everybody got full

memcheck:
	$(PREFIX)$(MEMCHECK) ./$(NAME) 2 800 200 200 1

racecheck:
	$(PREFIX)$(DATARACE) ./$(NAME) 1 800 200 200 1

test4:
	#everybody lives
	$(PREFIX)./$(NAME) 4 410 200 200
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
	$(PREFIX)eval $(test6_cmd)
	#both lived until you killed them

test7:
	#many live
	$(PREFIX)./$(NAME) 150 800 200 100
	#they all lived until you killed them


test8:
	#HANG
	$(PREFIX)./$(NAME) 5 800 200 200 7
	#Y U HANG

invalid_arg11 = ./$(NAME) a 1 2 3
invalid_arg12 = ./$(NAME) -1 1 2 3
invalid_arg13 = ./$(NAME) 999999999999999 1 2 3
invalid_arg21 = ./$(NAME) 1 a 1 2
invalid_arg22 = ./$(NAME) 1 -1 1 2
invalid_arg23 = ./$(NAME) 1 999999999999999 1 2
invalid_arg31 = ./$(NAME) 1 2 a 1
invalid_arg32 = ./$(NAME) 1 2 -1 1
invalid_arg33 = ./$(NAME) 1 2 999999999999999 1
invalid_arg41 = ./$(NAME) 1 2 3 a
invalid_arg42 = ./$(NAME) 1 2 3 -1
invalid_arg43 = ./$(NAME) 1 2 3 999999999999999
invalid_arg51 = ./$(NAME) 1 2 3 4 a
invalid_arg52 = ./$(NAME) 1 2 3 4 -1
invalid_arg53 = ./$(NAME) 1 2 3 4 999999999999999

invalid_args:
	-$(PREFIX)$(MEMCHECK) $(invalid_arg11)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg12)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg13)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg21)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg22)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg23)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg31)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg32)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg33)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg41)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg42)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg43)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg51)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg52)
	-$(PREFIX)$(MEMCHECK) $(invalid_arg53)

PHONY: all clean fclean re
