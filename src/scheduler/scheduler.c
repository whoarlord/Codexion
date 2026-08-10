/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 16:44:17 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/10 15:57:13 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	send_request(t_coder *coder)
{
	pthread_mutex_lock(&coder->flags->heap->mutex);
	if (check_dead(coder->flags))
		return (pthread_mutex_unlock(&coder->flags->heap->mutex), 1);
	heap_push(coder->flags, coder);
	coder->go_out = 0;
	while (coder->go_out != 1 && !check_dead(coder->flags))
		pthread_cond_wait(&coder->flags->heap->cond,
			&coder->flags->heap->mutex);
	if (check_dead(coder->flags))
		return (pthread_mutex_unlock(&coder->flags->heap->mutex), 1);
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

void	*scheduler_loop(void *flags_pointer)
{
	t_flags	*flags;

	flags = (t_flags *)flags_pointer;
	while (!check_dead(flags))
	{
		usleep(1000);
		if (check_dead(flags))
			break ;
		printf("after\n");
		pthread_mutex_lock(&flags->heap->mutex);
		printf("before\n");
		if (flags->heap->size > 0)
			clear_heap(flags);
		pthread_mutex_unlock(&flags->heap->mutex);
		pthread_cond_broadcast(&flags->heap->cond);
	}
	pthread_cond_broadcast(&flags->heap->cond);
	return (NULL);
}
