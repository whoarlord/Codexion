/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 14:27:01 by iarrien-          #+#    #+#             */
/*   Updated: 2026/03/30 17:02:37 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	free_coder_from_queue(t_coder *coder)
{
	int		i;
	int		temp;
	t_queue	*queue;

	i = 0;
	temp = 0;
	queue = coder->flags->queue;
	queue->free_dongles[coder->right->id] = 0;
	queue->free_dongles[coder->left->id] = 0;
	while (i < coder->flags->number_of_coders - 1)
	{
		temp = queue->coders[i];
		queue->coders[i] = queue->coders[i + 1];
		queue->coders[i + 1] = temp;
		i++;
	}
	queue->coders[i] = 0;
	pthread_cond_broadcast(&queue->cond);
}

void	wait_till_cooldown(t_coder *coder)
{
	if (coder->left->last_use == 0)
		coder->left->last_use -= coder->flags->dongle_cooldown;
	if (coder->right->last_use == 0)
		coder->right->last_use -= coder->flags->dongle_cooldown;
	while (coder->left->last_use
		+ coder->flags->dongle_cooldown
		>= calculate_time(coder->flags->start_time)
		|| coder->right->last_use
		+ coder->flags->dongle_cooldown
		>= calculate_time(coder->flags->start_time))
		;
}

// A value 0 for the dongle of the array free_dongles means that the dongle is ready to use
int	fifo_queue(t_coder *coder)
{
	int		i;
	t_queue	*queue;

	i = 0;
	queue = coder->flags->queue;
	while (i < coder->flags->number_of_coders)
	{
		if (queue->coders[i] == 0)
		{
			queue->coders[i] = coder->number;
			break ;
		}
		i++;
	}
	while ((queue->free_dongles[coder->right->id] != 0
			|| queue->free_dongles[coder->left->id] != 0))
	{
		pthread_cond_wait(&queue->cond, &queue->mutex);
		if (check_dead(coder))
			return (pthread_mutex_unlock(&queue->mutex),
				pthread_cond_broadcast(&queue->cond), 1);
	}
	wait_till_cooldown(coder);
	if (check_dead(coder))
		return (pthread_mutex_unlock(&queue->mutex), 1);
	queue->free_dongles[coder->left->id] = 1;
	queue->free_dongles[coder->right->id] = 1;
	pthread_mutex_unlock(&queue->mutex);
	return (0);
}
