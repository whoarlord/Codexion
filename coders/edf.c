/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 11:02:20 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/16 17:47:06 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


void	update_queue_edf(t_coder *coder, t_queue *queue)
{
	int			i;
	int			burnout;

	burnout = coder->last_compile + coder->flags->time_to_burnout;
	i = 0;
	while (i < 2)
	{
		if (queue->coders[i] == 0 || queue->edf_priority_array[i] > burnout)
		{
			if (queue->edf_priority_array[i] > burnout)
				shift_right(queue);
			queue->coders[i] = coder->number;
			queue->edf_priority_array[i] = burnout;
			pthread_cond_signal(&queue->cond);
			break ;
		}
		i++;
	}
	/* for (size_t i = 0; i < sizeof(queue); i++)
		printf("%02x", ((unsigned char *)&queue)[i]);
	for (size_t i = 0; i < 2; i++) {
		printf(", index: %ld, %d", i, queue->edf_priority_array[i]);
	}
	for (size_t i = 0; i < 2; i++) {
		printf(", coders: %d", queue->coders[i]);
	}
	printf("\n"); */
}

void	lock(t_coder *coder, t_queue *queue)
{
	pthread_mutex_lock(&queue->mutex);
	update_queue_edf(coder, queue);
	while (queue->coders[0] != coder->number) {
		pthread_cond_wait(&queue->cond, &queue->mutex);
	}
	pthread_mutex_unlock(&queue->mutex);
}



int	edf_queue(t_coder *coder)
{
	t_queue	*first;
	t_queue	*second;
	int		coder_number;

	if (coder->right < coder->left) {
		first = coder->right->queue;
		second = coder->left->queue;
	} else {
		first = coder->left->queue;
		second = coder->right->queue;
	}
	coder_number = coder->number;
	while (first->coders[0] != coder_number
		&& second->coders[0] != coder_number) {
		lock(coder, first);
		if (check_dead(coder))
			return (pthread_cond_broadcast(&first->cond), 1);
		if (first->coders[0] == coder_number
			&& second->coders[0] == coder_number)
			break;
		lock(coder, second);
		if (check_dead(coder))
			return (pthread_cond_broadcast(&second->cond), 1);
	}
	first->is_busy = 1;
	second->is_busy = 1;
	wait_till_cooldown(coder);
	if (check_dead(coder))
		return (1);
	return (0);
}
