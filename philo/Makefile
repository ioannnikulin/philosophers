CC = cc
NAME = philo
PREFIX = 
COMPILE_FLAGS = -Wall -Wextra -Werror -pthread
LINK_FLAGS = -pthread
INCLUDES = -I.
MEMCHECK = valgrind --leak-check=full
DATARACE = valgrind --tool=helgrind 

all: $(NAME)

SRCS = init.c philo.c report.c setup.c utils.c main.c finalize.c wrappers.c t_s_ull.c t_s_ull_math.c t_s_ull_math_1.c t_s_usec.c monitor.c time.c print.c print_utils.c state_description.c philo_eating.c die_and_drop_forks.c philo_ret.c philo_smart_wait.c philo_forks.c t_s_ull_w.c atoui.c philo_sleep.c philo_birth.c
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

#====================================================

fulltest: test1 racecheck1 memcheck2 racecheck2 test3

#====================================================

test1_cmd = ./$(NAME) 1 800 200 200

test1:
	#shouldn't eat a single time and dies
	$(PREFIX)eval $(test1_cmd)
	#didn't eat a single time and died

debug1:
	$(PREFIX)gdbtui --args $(test1_cmd)

memcheck1:
	$(PREFIX)$(MEMCHECK) $(test1_cmd)

racecheck1:
	$(PREFIX)$(DATARACE) $(test1_cmd)

#====================================================

test2_cmd = ./$(NAME) 5 800 200 200

test2:
	#everybody lives
	$(PREFIX)eval $(test2_cmd)
	#everybody lived until you killed them

debug2:
	$(PREFIX)gdbtui --args $(test2_cmd)

memcheck2:
	$(PREFIX)$(MEMCHECK) $(test2_cmd)

racecheck2:
	$(PREFIX)$(DATARACE) $(test2_cmd)

#====================================================

test3_cmd = ./$(NAME) 5 800 200 200 7

test3:
	#everybody lives until they've eaten 7 times
	$(PREFIX)eval $(test3_cmd)
	#everybody got full

memcheck3:
	$(PREFIX)$(MEMCHECK) $(test3_cmd)

racecheck3:
	$(PREFIX)$(DATARACE) $(test3_cmd)

#====================================================

test4:
	#everybody lives
	$(PREFIX)./$(NAME) 4 410 200 200
	#everybody lived until you killed them

memcheck4:
	$(PREFIX)$(MEMCHECK) $(test4_cmd)

racecheck4:
	$(PREFIX)$(DATARACE) $(test4_cmd)

#====================================================

test5_cmd = ./$(NAME) 3 310 200 100

test5:
	#one dies
	$(PREFIX)eval $(test5_cmd)
	#one died

memcheck5:
	$(PREFIX)$(MEMCHECK) $(test5_cmd)

racecheck5:
	$(PREFIX)$(DATARACE) $(test5_cmd)

#====================================================

test6_cmd = ./$(NAME) 2 800 200 100

debug6:
	gdbtui --args $(test6_cmd)

test6:
	#both live
	$(PREFIX)eval $(test6_cmd)
	#both lived until you killed them

memcheck6:
	$(PREFIX)$(MEMCHECK) $(test6_cmd)

racecheck6:
	$(PREFIX)$(DATARACE) $(test6_cmd)

#====================================================

test7_cmd = ./$(NAME) 150 800 200 100

test7:
	#many live
	$(PREFIX)eval $(test7_cmd)
	#they all lived until you killed them

memcheck7:
	$(PREFIX)$(MEMCHECK) $(test7_cmd)

racecheck7:
	$(PREFIX)$(DATARACE) $(test7_cmd)

#====================================================

test8_cmd = ./$(NAME) 5 800 200 200 7

test8:
	#used go hang
	$(PREFIX)eval $(test8_cmd)
	#ok now

memcheck8:
	$(PREFIX)$(MEMCHECK) $(test8_cmd)

racecheck8:
	$(PREFIX)$(DATARACE) $(test8_cmd)

#====================================================

test9_cmd = ./$(NAME) 4 800 200 200 5

test9:
	#
	$(PREFIX)eval $(test9_cmd)
	#

memcheck9:
	$(PREFIX)$(MEMCHECK) $(test9_cmd)

racecheck9:
	$(PREFIX)$(DATARACE) $(test9_cmd)

#====================================================

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
