/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:28:13 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/23 17:04:51 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	check_coder_index(int *coders, int actual_coder_number, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (coders[i] == actual_coder_number)
			return (i);
		i++;
	}
	return (i);
}

void	shift_left(t_queue *queue)
{
	int		coders_temp;
	int		burnout_temp;

	coders_temp = queue->coders[0];
	queue->coders[0] = queue->coders[1];
	queue->coders[1] = coders_temp;
	burnout_temp = queue->edf_priority_array[0];
	queue->edf_priority_array[0] = queue->edf_priority_array[1];
	queue->edf_priority_array[1] = burnout_temp;
	queue->coders[1] = 0;
	queue->edf_priority_array[1] = 0;
}

void	shift_right(t_queue *queue)
{
	queue->coders[1] = queue->coders[0];
	queue->edf_priority_array[1] = queue->edf_priority_array[0];
	queue->coders[0] = 0;
	queue->edf_priority_array[0] = 0;
}

void	free_coder_from_queue(t_coder *coder)
{
	shift_left(coder->left->queue);
	shift_left(coder->right->queue);
}


void	update_coders_queue(t_coder *coder)
{
	t_dongle	*right;
	t_dongle	*left;

	right = coder->right;
	left = coder->left;

	pthread_mutex_lock(&right->mutex);
	shift_left(right->queue);
	right->queue->is_busy = 0;
	pthread_mutex_unlock(&right->mutex);

	pthread_mutex_lock(&left->mutex);
	shift_left(left->queue);
	left->queue->is_busy = 0;
	pthread_mutex_unlock(&left->mutex);

	pthread_cond_broadcast(&left->cond);
	pthread_cond_broadcast(&right->cond);
}
