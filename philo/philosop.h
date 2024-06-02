/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/06/02 19:31:33 by inikulin         ###   ########.fr       */
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

# define TAKES 1
# define EATS 2
# define SLEEPS 3
# define THINKS 4
# define DIES 5

# define DEAD_REPORT_MICROS 10000
# define MAX_MICROS 2000000000

struct				s_philo;

typedef long long	t_usec;

typedef struct s_props
{
	unsigned int	sz;
	struct s_philo	*philos;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_poll;
	int				enough;
	t_usec			tstart;
	unsigned int	full_philos;
	int				errno;
}	t_props;

typedef struct s_philo
{
	int				i;
	pthread_mutex_t	*l;
	pthread_mutex_t	*r;
	int				times_eaten;
	pthread_mutex_t	m_times_eaten;
	int				dead;
	pthread_mutex_t	m_dead;
	t_usec			wait;
	t_usec			tdie;
	t_usec			teat;
	t_usec			tsleep;
	t_usec			last_meal;
	pthread_mutex_t	m_last_meal;
	int				full_tgt;
	time_t			delta;
	t_props			*props;
}	t_philo;

int		usage(int ret);
int		init(t_props *p, int argc, char **argv);
int		setup(t_props *p);
t_usec	mtime(t_usec *t, int *ok);
void	*philo(void *arg);
int		report(t_philo *p, int action, t_usec t);
int		done(t_props *p, int print_unlock, char *msg, int ret);
int		finalize(t_props *p, char *msg, int ret);
int		assign(int *to, int val, int ret);
#endif
