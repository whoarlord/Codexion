/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 16:44:17 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/07 17:19:00 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	print_queue(t_heap *heap)
{
	int	i;

	printf("---- QUEUE (length=%d, size=%d) ----\n",
		heap->length, heap->size);
	i = 0;
	while (i < heap->size)
	{
		printf("[%d] coder=%d | arrival=%lld | time_to_burnout=%d | "
			"compile_count=%d | next_event_time=%d | go_out=%d\n",
			i,
			heap->queue[i].coder->number,
			heap->queue[i].arrival,
			heap->queue[i].coder->last_compile + heap->queue[i].coder->flags->time_to_burnout,
			heap->queue[i].coder->compile_count,
			heap->queue[i].coder->next_event_time,
			heap->queue[i].coder->go_out);
		i++;
	}
	printf("-------------------------------------\n");
}

int		send_request(t_coder *coder) {
	pthread_mutex_lock(&coder->flags->heap->mutex);
	heap_push(coder->flags, coder);
	coder->go_out = 0;
	while (coder->go_out != 1) {
		if (check_dead(coder->flags))
			return (1);
		pthread_cond_wait(&coder->flags->heap->cond,
			&coder->flags->heap->mutex);
	}
	pthread_mutex_unlock(&coder->flags->heap->mutex);
	return (0);
}

void	get_timeout(struct timespec *ts, long long ms)
{
	struct timeval	tv;
	long long		usec;

	gettimeofday(&tv, NULL);
	usec = tv.tv_usec + (ms % 1000) * 1000;
	ts->tv_sec = tv.tv_sec + (ms / 1000) + (usec / 1000000);
	ts->tv_nsec = (usec % 1000000) * 1000;
}

void	*scheduler_loop(void *flags_pointer) {
	t_flags	*flags;
	//struct timespec ts;

	flags = (t_flags*) flags_pointer;
	if (flags->number_of_coders == 1)
		return (NULL);
	while (!check_dead(flags)) {
		usleep(1000);
		if (check_dead(flags)) {
			break;
		}
		pthread_mutex_lock(&flags->heap->mutex);
		if (flags->heap->size > 0) {
			clear_heap(flags);
		}
		pthread_mutex_unlock(&flags->heap->mutex);
		pthread_cond_broadcast(&flags->heap->cond);
	}
	pthread_cond_broadcast(&flags->heap->cond);
	return (NULL);
}
