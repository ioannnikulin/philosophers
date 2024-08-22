/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resources.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inikulin <inikulin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:10:05 by inikulin          #+#    #+#             */
/*   Updated: 2024/08/22 20:32:04 by inikulin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCES_H
# define RESOURCES_H

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

# define TX_LOOKS "starts looking for forks"
# define TX_BEFORE_INSPECTION "will be inspected now"
# define TX_INSIDE_INSPECTION "is being inspected"
#endif
