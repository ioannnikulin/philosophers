/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/26 20:38:34 by inikulin         ###   ########.fr       */
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

# define TAKES 1
# define EATS 2
# define SLEEPS 3
# define THINKS 4
# define DIES 5

# define DEAD_REPORT_MICROS 10000
# define MAX_MICROS 2000000000

struct	s_philo;

typedef struct s_props
{
	unsigned int	sz;
	t_phil_props	phil_props;
	struct s_philo	*philos;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_poll;
	int				enough;
	time_t			time_start;
	unsigned int	full_philos;
}	t_props;

typedef struct s_philo
{
	int				i;
	pthread_mutex_t	*l;
	pthread_mutex_t	*r;
	int				times_eaten;
	time_t			wait;
	time_t			tdie;
	time_t			teat;
	time_t			tsleep;
	time_t			last_meal;
	int				full_tgt;
	time_t			delta;
	t_props			*props;
}	t_philo;

int		usage(int ret);
int		init(t_props *p, int argc, char **argv);
int		setup(t_props *p);
time_t	time(time_t *t, int *ok);
int		philo(void *arg);
int		report(t_philo *p, int action, int t);
int		done(t_props *p, int print_unlock, char *msg, int ret);
int		finalize(t_props *p, char *msg, int ret);
#endif
