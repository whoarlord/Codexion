/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:02:20 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/14 18:35:56 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


void	update_queue_edf(t_coder *coder, t_dongle *dongle)
{
	int			i;
	int			n;
	int			burnout;
	t_queue		*queue;

	i = 0;
	n = coder->flags->number_of_coders;
	burnout = coder->last_compile + coder->flags->time_to_burnout;
	queue = dongle->queue;
	while (i < n)
	{
		if (queue->coders[i] == 0 || queue->edf_min_value > burnout)
		{
			if (queue->edf_min_value > burnout)
				shift_right(queue, i, coder->flags->number_of_coders);
			queue->coders[i] = coder->number;
			queue->edf_min_value = burnout;
			break ;
		}
		i++;
	}
}



int	edf_queue(t_coder *coder)
{
	t_queue	*right_queue;

	right_queue = coder->right->queue;
	pthread_mutex_lock(&right_queue->mutex);
	update_queue_edf(coder, right_queue);
	pthread_mutex_unlock(&right_queue->mutex);
	while ((check_before_coders(queue->coders, queue->free_dongles, coder)
			|| queue->free_dongles[coder->right->id] != 0
			|| queue->free_dongles[coder->left->id] != 0))
	{
		if (check_dead(coder))
			return (pthread_mutex_unlock(&queue->mutex),
				pthread_cond_broadcast(&queue->cond), 1);
		pthread_cond_wait(&queue->cond, &queue->mutex);
	}
	queue->free_dongles[coder->left->id] = 1;
	queue->free_dongles[coder->right->id] = 1;
	free_coder_from_queue(coder);
	pthread_mutex_unlock(&queue->mutex);
	pthread_cond_broadcast(&queue->cond);
	wait_till_cooldown(coder);
	if (check_dead(coder))
		return (1);
	return (0);
}
