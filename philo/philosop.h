/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/22 20:33:22 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOP_H
# define PHILOSOP_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include "utils.h"
# include "resources.h"

# define MONITOR

# define NEWBORN 1
# define TAKES 2
# define EATS 4
# define SLEEPS 8
# define THINKS 16
# define DIES 32
# define ENOUGH 64
# define TOOK_L 128
# define TOOK_R 256
# define TOOK_BOTH 384
# define ANY_ALIVE 31
# define ANY_UNALIVE 96
# define ANY 1023

# define LOOKS 512
# define BEFORE_INSPECTION 1024
# define INSIDE_INSPECTION 2048

# define DELAY 500LL

# define DEAD_REPORT_MICROS 10000LL
# define MAX_MICROS 2000000LL

# define FREE_PHILOS 1 /* make sure to DESTROY_M_IN_PHILO */
# define FREE_THREADS 2
# define FREE_FORKS 4 /* make sure to DESTROY_M_FORKS */
# define UNLOCK_PRINT 8
# define DESTROY_M_FORKS 16
# define DESTROY_M_ENOUGH 32
# define DESTROY_M_LAST_MEAL 64
# define DESTROY_M_STATE 128
# define DESTROY_M_IN_PHILO (128 + 64)
# define DESTROY_M_PRINT 256
# define DESTROY_M_TIME 2048
# define REPORT_FATAL 512 /* don't combine with anything except for UNLOCK_PRINT */
# define STAGE_1 (1 + 2 + 4 + 1024)
# define STAGE_2 (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 1024)

struct					s_philo;

typedef struct s_props
{
	unsigned int	sz;
	struct s_philo	*philos;
	pthread_t		*threads;
	t_mutex			*forks;
	t_mutex			print_poll;
	t_mutex			mtime;
	t_s_int			enough;
	t_usec			tstart;
	unsigned int	full_philos; /* controlled by print_poll too */
	int				errno;
	pthread_t		monitor;
}	t_props;

typedef struct s_philo
{
	int			i; 
	/*
	** initially -1 to mark uninitialized.
	** if initalization for later philos fails,
	** we know that non-neg here means that we should free it
	*/
	t_mutex		*l;
	t_mutex		*r;
	int			times_eaten;
	t_s_int		state;
	t_usec		wait;
	t_usec		tdie;
	t_usec		teat;
	t_usec		tsleep;
	t_s_usec	last_meal;
	/*
	 ** initially -1,
	 ** set to 0 in setup()
	 ** don't remember what for though :(
	 */
	int			full_tgt;
	time_t		delta;
	t_props		*props;
}	t_philo;

typedef struct s_fin_param
{
	char	*msg;
	t_usec	time;
}	t_fin_param;

int			usage(int ret);
int			init(t_props *p, int argc, char **argv);
int			setup(t_props *p);
void		*philo(void *arg);
void		*moni(void *a);
int			report(t_philo *p, int action, t_usec t);
int			finalize(t_props *p, int mode, t_fin_param msg, int ret);
t_fin_param	msg(char *msg, t_usec time);

char		*state_description(int state);

t_usec		mtime(t_usec *t, int *ok, t_props *p);
void		msleep(t_usec t, t_props *p);
void		print(long long int printed, long long int happened, t_philo* p, int state);

t_usec		prepare_to_eat(t_philo *p, int *errno);
t_usec		eat(t_philo *p, int *errno, t_usec before);
int			die_and_drop_forks(t_philo *p);
#endif
