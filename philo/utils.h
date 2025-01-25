/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2025/01/25 21:19:04 by inikulin         ###   ########.fr       */
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

typedef long long			t_usec;
typedef pthread_mutex_t		t_mutex;
typedef unsigned long long	t_ull;

typedef struct s_s_int
{
	int		v;
	t_mutex	m;
	char	*e_lock;
	char	*e_unlock;
}	t_s_int;

typedef struct s_s_ull
{
	t_ull	v;
	t_mutex	m;
	char	*e_lock;
	char	*e_unlock;
}	t_s_ull;

typedef struct s_s_usec
{
	t_usec	v;
	t_mutex	m;
	char	*e_lock;
	char	*e_unlock;
}	t_s_usec;

int				assign(int *to, int val, int ret);
int				mfree(int choice, void **w, int sz, int ret);
int				mbzero(void *f, int bytes, int ret);
void			*mcalloc(size_t sz);
char			*ifc(int choice, char *t, char *f);
unsigned int	atoui(const char *nptr, int *errno);

int				m_init(t_mutex *m);
int				m_kill(t_mutex *m);
int				m_lock(t_mutex *m);
int				m_unlock(t_mutex *m);

t_ull			tsull_get(t_s_ull *i, t_s_ull *errno);
t_ull			tsull_get_release(t_s_ull *i, t_s_ull *errno);
t_s_ull			*tsull_set(t_s_ull *i, t_ull *oldval, t_ull newval,
					t_s_ull *errno);
t_s_ull			*tsull_set_release(t_s_ull *i, t_ull *oldval, t_ull newval, 
					t_s_ull *errno);
t_s_ull			*tsull_add(t_s_ull *i, t_ull val, t_s_ull *errno);
t_s_ull			*tsull_or(t_s_ull *i, t_ull val, t_s_ull *errno);
t_s_ull			*tsull_or_release(t_s_ull *i, t_ull val, t_s_ull *errno);
t_s_ull			*tsull_nand(t_s_ull *i, t_ull val, t_s_ull *errno);
t_s_ull			*tsull_nand_release(t_s_ull *i, t_ull val, t_s_ull *errno);
int				tsull_release(t_s_ull *i, t_s_ull *errno);

t_usec			tsusec_get(t_s_usec *i, t_s_ull *errno);
t_s_usec		*tsusec_set(t_s_usec *i, t_usec val, t_s_ull *errno);
t_s_usec		*tsusec_add(t_s_usec *i, t_usec val, t_s_ull *errno);

int				ret_errno(t_s_ull *errno, int val, int ret);

int				prints(char *s, int ret);
int				printlli(long long int i, int ret);
int				printull(unsigned long long i, int ret);
int				ull(unsigned long long int i, int sign);
#endif
