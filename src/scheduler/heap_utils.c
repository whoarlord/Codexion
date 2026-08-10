/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:12:33 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/10 15:58:38 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

t_request	heap_pop(t_flags *flags)
{
	t_heap		*heap;
	t_request	request;

	heap = flags->heap;
	request = heap->queue[0];
	heap->size--;
	if (heap->size != 0)
		heap->queue[0] = heap->queue[heap->size];
	heapify_down(flags);
	return (request);
}

void	heap_push(t_flags *flags, t_coder *coder)
{
	t_heap	*heap;

	heap = flags->heap;
	//printf("here coder: %d\n", coder->number);
	if (heap->size < heap->length)
	{
		heap->queue[heap->size].coder = coder;
		heap->queue[heap->size].arrival = heap->last_arrival;
		heap->queue[heap->size].time_to_burnout = (coder->last_compile
				+ flags->time_to_burnout);
		heap->last_arrival++;
		heap->size++;
		heapify_up(flags);
	}
}

void	heap_push_request(t_flags *flags, t_request request)
{
	t_heap	*heap;

	heap = flags->heap;
	if (heap->size < heap->length)
	{
		heap->queue[heap->size] = request;
		heap->size++;
		heapify_up(flags);
	}
}

void	popping_out_everything(t_flags *flags, int *going_out_index,
								int *staying_index)
{
	t_request	request;
	t_heap		*heap;

	heap = flags->heap;
	while (heap->size > 0)
	{
		request = heap_pop(flags);
		if (request.coder->right->is_busy || request.coder->left->is_busy)
		{
			heap->staying[*staying_index] = request;
			*staying_index = *staying_index + 1;
		}
		else
		{
			heap->going_out[*going_out_index] = request;
			request.coder->left->is_busy = 1;
			request.coder->right->is_busy = 1;
			*going_out_index = *going_out_index + 1;
		}
	}
}

void	clear_heap(t_flags *flags)
{
	t_request	request;
	t_heap		*heap;
	int			going_out_index;
	int			staying_index;
	int			i;

	heap = flags->heap;
	going_out_index = 0;
	staying_index = 0;
	popping_out_everything(flags, &going_out_index, &staying_index);
	i = 0;
	while (i < going_out_index)
	{
		request = heap->going_out[i];
		request.coder->go_out = 1;
		i++;
	}
	i = 0;
	while (i < staying_index)
	{
		heap_push_request(flags, heap->staying[i]);
		i++;
	}
}
