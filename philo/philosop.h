/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/19 21:59:07 by inikulin         ###   ########.fr       */
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
# define TOOK_BOTH 512
# define ANY_ALIVE 927
# define ANY_UNALIVE 96
# define ANY 1023

# define DELAY 500LL

# define DEAD_REPORT_MICROS 10000LL
# define MAX_MICROS 2000LL

# define TX_ERR_MALLOC "Error allocating memory"
# define TX_ERR_TIMER "Timer error"
# define TX_ERR_SLEEP "Insomnia"
# define TX_ERR_NUM_FORMAT "Unexpected number format"
# define TX_ERR_MUTEX_INIT "Mutex initialization failure"
# define TX_ERR_MUTEX_PRINT_LOCK "Print mutex lock failure"
# define TX_ERR_MUTEX_PRINT_UNLOCK "Print mutex unlock failure"
# define TX_ERR_MUTEX_STATE_LOCK "Simulation state mutex lock failure"
# define TX_ERR_MUTEX_STATE_UNLOCK "Simulation state mutex unlock failure"
# define TX_ERR_MUTEX_IND_STATE_LOCK "Philosopher state mutex lock failure"
# define TX_ERR_MUTEX_IND_STATE_UNLOCK "Philosopher state mutex unlock failure"
# define TX_ERR_MUTEX_IND_LAST_MEAL_LOCK "Philosopher last meal mutex lock failure"
# define TX_ERR_MUTEX_IND_LAST_MEAL_UNLOCK "Philosopher last meal mutex unlock \
	failure"
# define TX_ERR_MUTEX_TIMER_LOCK "Failed to lock timer mutex"
# define TX_ERR_MUTEX_TIMER_UNLOCK "Failed to unlock timer mutex"
# define TX_ERR_MUTEX_KILL "Mutex destruction failure"
# define TX_ERR_THREAD_START "Failed to start a thread"
# define TX_ERR_THREAD_JOIN "Failed to join a thread"
# define TX_ERR_FORK_TAKE "Failed to take a fork"
# define TX_ERR_FORK_PUT "Failed to put a fork"
# define TX_ERR_BIRTH "Failed to initialize a philosopher"
# define TX_MAX_TIME "Simulation time limit reached"
# define TX_OVER "Simulation over"
# define TX_FULL "Everybody has eaten enough"
# define TX_LATE "Death was reported too late"
# define TX_NEWBORN "hasn't been born yet"
# define TX_TAKES "has taken a fork"
# define TX_EATS "is eating"
# define TX_SLEEPS "is sleeping"
# define TX_THINKS "is thinking"
# define TX_DIES "died"
# define TX_ENOUGH "leaves the table full"

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

typedef long long		t_usec;
typedef pthread_mutex_t	t_mutex;

typedef struct s_s_int
{
	int		v;
	t_mutex	m;
	char	*e_lock;
	char	*e_unlock;
}	t_s_int;

typedef struct s_s_usec
{
	t_usec	v;
	t_mutex	m;
	char	*e_lock;
	char	*e_unlock;
}	t_s_usec;

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
t_usec		mtime(t_usec *t, int *ok, t_props *p);
void		msleep(t_usec t, t_props *p);
void		*philo(void *arg);
void		*moni(void *a);
int			report(t_philo *p, int action, t_usec t);
int			finalize(t_props *p, int mode, t_fin_param msg, int ret);
t_fin_param	msg(char *msg, t_usec time);
int			assign(int *to, int val, int ret);
int			mfree(int choice, void **w, int sz, int ret);
int			mbzero(void *f, int bytes, int ret);
void		*mcalloc(size_t sz);

int			m_init(t_mutex *m);
int			m_kill(t_mutex *m);
int			m_lock(t_mutex *m);
int			m_unlock(t_mutex *m);

int			tsint_get(t_s_int *i, int *errno);
t_s_int		*tsint_set(t_s_int *i, int oldval, int newval, int *errno);
t_s_int		*tsint_add(t_s_int *i, int val, int *errno);
t_usec		tsusec_get(t_s_usec *i, int *errno);
t_s_usec	*tsusec_set(t_s_usec *i, t_usec val, int *errno);
t_s_usec	*tsusec_add(t_s_usec *i, t_usec val, int *errno);

int			prints(char *s, int ret);
int			printlli(long long int i, int ret);
void		print(long long int printed, long long int happened, t_philo* p, int state);
char		*state_description(int state);
#endif
