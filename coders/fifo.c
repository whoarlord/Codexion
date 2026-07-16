/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 14:27:01 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/15 16:58:42 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


void	update_queue_fifo(t_coder *coder, t_queue *queue)
{
	int			i;

	i = 0;
	while (i < 2)
	{
		if (queue->coders[i] == 0)
		{
			queue->coders[i] = coder->number;
			break ;
		}
		i++;
	}
}


// A value 0 for the dongle of the array free_
// dongles means that the dongle is ready to use
int	fifo_queue(t_coder *coder)
{
	t_queue	*right_queue;
	t_queue	*left_queue;

	right_queue = coder->right->queue;
	left_queue = coder->left->queue;
	while (right_queue->coders[0] != coder->number
		&& left_queue->coders[0] != coder->number) {
		pthread_mutex_lock(&right_queue->mutex);
		update_queue_fifo(coder, right_queue);
		pthread_mutex_unlock(&right_queue->mutex);
		if (check_dead(coder))
			return (pthread_mutex_unlock(&right_queue->mutex),
				pthread_cond_broadcast(&right_queue->cond), 1);
		pthread_cond_wait(&right_queue->cond, &right_queue->mutex);

		pthread_mutex_lock(&left_queue->mutex);
		update_queue_fifo(coder, left_queue);
		pthread_mutex_unlock(&left_queue->mutex);
		if (check_dead(coder))
			return (pthread_mutex_unlock(&left_queue->mutex),
				pthread_cond_broadcast(&left_queue->cond), 1);
		pthread_cond_wait(&left_queue->cond, &left_queue->mutex);
	}
	wait_till_cooldown(coder);
	if (check_dead(coder))
		return (1);
	return (0);
}
