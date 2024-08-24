/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/24 15:46:39 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

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

int			assign(int *to, int val, int ret);
int			mfree(int choice, void **w, int sz, int ret);
int			mbzero(void *f, int bytes, int ret);
void		*mcalloc(size_t sz);

int			m_init(t_mutex *m);
int			m_kill(t_mutex *m);
int			m_lock(t_mutex *m);
int			m_unlock(t_mutex *m);

int			tsint_get(t_s_int *i, int *errno);
int			tsint_get_release(t_s_int *i, int *errno);
t_s_int		*tsint_set(t_s_int *i, int oldval, int newval, int *errno);
t_s_int		*tsint_set_release(t_s_int *i, int oldval, int newval, int *errno);
t_s_int		*tsint_add(t_s_int *i, int val, int *errno);
t_s_int		*tsint_or(t_s_int *i, int val, int *errno);
t_s_int		*tsint_or_release(t_s_int *i, int val, int *errno);
t_s_int		*tsint_nor(t_s_int *i, int val, int *errno);
t_s_int		*tsint_nor_release(t_s_int *i, int val, int *errno);
int			tsint_release(t_s_int *i, int *errno);
t_usec		tsusec_get(t_s_usec *i, int *errno);
t_s_usec	*tsusec_set(t_s_usec *i, t_usec val, int *errno);
t_s_usec	*tsusec_add(t_s_usec *i, t_usec val, int *errno);

int			prints(char *s, int ret);
int			printlli(long long int i, int ret);
#endif
