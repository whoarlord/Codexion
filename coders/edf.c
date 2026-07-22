/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:02:20 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/22 17:15:38 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


void	update_queue_edf(t_coder *coder, t_dongle *dongle)
{
	int			i;
	int			burnout;
	t_queue		*queue;

	burnout = coder->last_compile + coder->flags->time_to_burnout;
	i = 0;
	queue = dongle->queue;
	while (i < 2)
	{
		if (queue->coders[i] == 0 || queue->edf_priority_array[i] > burnout)
		{
			if (queue->edf_priority_array[i] > burnout)
				shift_right(queue);
			queue->coders[i] = coder->number;
			queue->edf_priority_array[i] = burnout;
			pthread_cond_signal(&dongle->cond);
			break ;
		}
		i++;
	}
}

void	lock(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	update_queue_edf(coder, dongle);
	while (dongle->queue->coders[0] != coder->number) {
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	pthread_mutex_unlock(&dongle->mutex);
}



int	edf_queue(t_coder *coder)
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
