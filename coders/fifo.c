/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 14:27:01 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/22 17:34:52 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


void	update_queue_fifo(t_coder *coder, t_dongle *dongle)
{
	int			i;
	t_queue		*queue;

	i = 0;
	queue = dongle->queue;
	while (i < 2)
	{
		if (queue->coders[i] == 0)
		{
			shift_right(queue);
			queue->coders[i] = coder->number;
			pthread_cond_signal(&dongle->cond);
			break ;
		}
		i++;
	}
}

static void	lock(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	update_queue_fifo(coder, dongle);
	while (dongle->queue->coders[0] != coder->number) {
		if (check_dead(coder))
			break ;
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
}



int	fifo_queue(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int		coder_number;

	if (coder->right < coder->left) {
		first = coder->right;
		second = coder->left;
	} else {
		first = coder->left;
		second = coder->right;
	}
	coder_number = coder->number;
	while (first->queue->coders[0] != coder_number
		&& second->queue->coders[0] != coder_number) {
		lock(coder, first);
		if (check_dead(coder))
			return (pthread_cond_broadcast(&first->cond), 1);
		if (first->queue->coders[0] == coder_number
			&& second->queue->coders[0] == coder_number)
			break;
		lock(coder, second);
		if (check_dead(coder))
			return (pthread_cond_broadcast(&second->cond), 1);
	}
	first->queue->is_busy = 1;
	second->queue->is_busy = 1;
	wait_till_cooldown(coder);
	if (check_dead(coder))
		return (1);
	return (0);
}
