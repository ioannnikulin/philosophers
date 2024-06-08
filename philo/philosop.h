/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/08 20:41:15 by inikulin         ###   ########.fr       */
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

# define NEWBORN 0
# define TAKES 1
# define EATS 2
# define SLEEPS 3
# define THINKS 4
# define DIES 5
# define ENOUGH 6

# define DEAD_REPORT_MICROS 10000
# define MAX_MICROS 2000000000

# define TX_ERR_MALLOC "Error allocating memory"
# define TX_ERR_TIMER "Timer error"
# define TX_ERR_NUM_FORMAT "Unexpected number format"
# define TX_ERR_MUTEX_INIT "Mutex initialization failure"
# define TX_ERR_MUTEX_PRINT_LOCK "Print mutex lock failure"
# define TX_ERR_MUTEX_PRINT_UNLOCK "Print mutex unlock failure"
# define TX_ERR_MUTEX_STATE_LOCK "Simulation state mutex lock failure"
# define TX_ERR_MUTEX_STATE_UNLOCK "Simulation state mutex unlock failure"
# define TX_ERR_MUTEX_IND_STATE_LOCK "Philosopher state mutex lock failure"
# define TX_ERR_MUTEX_IND_STATE_UNLOCK "Philosopher state mutex unlock failure"
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

# define FREE_PHILOS 1 // make sure to DESTROY_M_IN_PHILO
# define FREE_THREADS 2
# define FREE_FORKS 4 // make sure to DESTROY_M_FORKS
# define UNLOCK_PRINT 8
# define DESTROY_M_FORKS 16
# define DESTROY_M_DEADS 32
# define DESTROY_M_LAST_MEALS 64
# define DESTROY_M_TIMES_EATEN 128
# define DESTROY_M_IN_PHILO (128 + 64 + 32)
# define DESTROY_M_PRINT 256
# define REPORT_FATAL 512
# define STAGE_1 (1 + 2 + 4)
# define STAGE_2 (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256)

struct					s_philo;

typedef long long		t_usec;
typedef pthread_mutex_t	t_mutex;

typedef struct s_s_int
{
	int		v;
	t_mutex	m;
	int		e;
}	t_s_int;

typedef struct s_props
{
	unsigned int	sz;
	struct s_philo	*philos;
	pthread_t		*threads;
	t_mutex			*forks;
	t_mutex			print_poll;
	t_s_int			enough;
	t_usec			tstart;
	unsigned int	full_philos;
	int				errno;
}	t_props;

typedef struct s_philo
{
	int		i;
	t_mutex	*l;
	t_mutex	*r;
	t_s_int	times_eaten;
	t_s_int	state;
	t_usec	wait;
	t_usec	tdie;
	t_usec	teat;
	t_usec	tsleep;
	t_usec	last_meal;
	t_mutex	m_last_meal;
	int		full_tgt;
	time_t	delta;
	t_props	*props;
}	t_philo;

int		usage(int ret);
int		init(t_props *p, int argc, char **argv);
int		setup(t_props *p);
t_usec	mtime(t_usec *t, int *ok);
void	*philo(void *arg);
int		report(t_philo *p, int action, t_usec t);
int		finalize(t_props *p, char *msg, int ret);
int		assign(int *to, int val, int ret);
int		mfree(int choice, void **w, int sz, int ret);
int		mbzero(void *f, int bytes, int ret);

int		m_init(t_mutex *m);
int		m_kill(t_mutex *m);
#endif
