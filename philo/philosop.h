/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosop.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/05/25 20:09:12 by inikulin         ###   ########.fr       */
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

# define DEAD 0
# define SEARCH 1
# define STABLE 2

# define TAKES 1
# define EATS 2
# define SLEEPS 3
# define THINKS 4
# define DIES 5

# define DEAD_REPORT_MICROS 10000

typedef struct s_phil_props
{
	unsigned int	tdie;
	unsigned int	teat;
	unsigned int	tsleep;
	unsigned int	full_tgt;
	unsigned int	full_act;
}	t_phil_props;

typedef struct s_print
{
	pthread_mutex_t	poll;
	struct timeval	t;
	struct timeval	t1;
}	t_print;

struct	s_philo;

typedef struct s_props
{
	unsigned int	sz;
	t_phil_props	phil_props;
	struct s_philo	*philos;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	t_print			print;
}	t_props;

typedef struct s_philo
{
	int		i;
	int		strat;
	int		prevwait;
	t_props	*props;
}	t_philo;

int	report(t_props *p, int state);

#endif
