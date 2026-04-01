/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:28:13 by iarrien-          #+#    #+#             */
/*   Updated: 2026/04/01 12:10:36 by iarrien-         ###   ########.fr       */
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

void	shift_left(t_queue *queue, int i, int size)
{
	int		coders_temp;
	int		burnout_temp;

	coders_temp = 0;
	burnout_temp = 0;
	while (i < size - 1)
	{
		coders_temp = queue->coders[i];
		queue->coders[i] = queue->coders[i + 1];
		queue->coders[i + 1] = coders_temp;
		burnout_temp = queue->edf_priority_array[i];
		queue->edf_priority_array[i] = queue->edf_priority_array[i + 1];
		queue->edf_priority_array[i + 1] = burnout_temp;
		i++;
	}
	queue->coders[i] = 0;
	queue->edf_priority_array[i] = 0;
}

void	shift_right(t_queue *queue, int i, int size)
{
	while (size - 2 >= i)
	{
		queue->coders[size - 1] = queue->coders[size - 2];
		queue->edf_priority_array[size - 1] = queue->edf_priority_array[
			size - 2];
		size--;
	}
	queue->coders[i] = 0;
	queue->edf_priority_array[i] = 0;
}

void	free_coder_from_queue(t_coder *coder)
{
	int		i;

	i = check_coder_index(coder->flags->queue->coders, coder->number,
			coder->flags->number_of_coders);
	shift_left(coder->flags->queue, i, coder->flags->number_of_coders);
}


void	update_coders_queue(t_coder *coder)
{
	t_queue	*queue;

	queue = coder->flags->queue;
	queue->free_dongles[coder->right->id] = 0;
	queue->free_dongles[coder->left->id] = 0;
	pthread_cond_broadcast(&queue->cond);
}
