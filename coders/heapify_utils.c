/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heapify_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 16:05:08 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/07 16:47:10 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

//pendiente logica para edf
int	order_heap(t_flags *flags, t_request parent, t_request child) {
	if (flags->scheduler == fifo) {
		if (parent.arrival > child.arrival)
			return (1);
		else
			return (0);
	} else {
		if (parent.time_to_burnout > child.time_to_burnout)
			return (1);
		else if (parent.time_to_burnout == child.time_to_burnout)
			return (parent.coder->number > child.coder->number);
		else
			return (0);
	}
	return (0);
}

void	swap_heap(t_heap *heap, int parent_index, int child_index) {
	t_request	request;

	request = heap->queue[parent_index];
	heap->queue[parent_index] = heap->queue[child_index];
	heap->queue[child_index] = request;
}

void	heapify_up(t_flags *flags) {
	t_heap	*heap;
	int		i;
	int		result;

	heap = flags->heap;
	i = heap->size - 1;
	while (i > 0) {
		result = order_heap(flags, heap->queue[(i - 1) / 2], heap->queue[i]);
		if (result) {
			swap_heap(heap, (i - 1) / 2, i);
			i = (i - 1) / 2;
		} else {
			break ;
		}
	}
}

void	heapify_down(t_flags *flags)
{
	t_heap	*heap;
	int		i;
	int		left;
	int		right;
	int		smallest;

	heap = flags->heap;
	i = 0;
	while (1)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		smallest = i;
		if (left < heap->size
			&& order_heap(flags, heap->queue[smallest], heap->queue[left]) == 1)
			smallest = left;
		if (right < heap->size
			&& order_heap(flags, heap->queue[smallest], heap->queue[right]) == 1)
			smallest = right;
		if (smallest == i)
			break ;
		swap_heap(heap, i, smallest);
		i = smallest;
	}
}
