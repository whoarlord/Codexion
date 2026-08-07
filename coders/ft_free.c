/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 16:51:49 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/07 17:25:54 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	free_flags(t_flags *flags)
{
	pthread_mutex_destroy(&flags->print_mutex);
	pthread_mutex_destroy(&flags->dead_mutex);
	free(flags);
}

void	ft_free_everything(t_dongle **dongles, t_coder **coders, t_flags *flags)
{
	int	i;
	int	count;
	printf("freeing everything\n");

	i = -1;
	count = flags->number_of_coders;
	while (dongles && ++i < count)
	{
		if (!dongles[i])
			continue ;
		if (dongles[i]->mutex_ready)
			pthread_mutex_destroy(&dongles[i]->mutex);
		free(dongles[i]);
	}
	free(dongles);
	i = -1;
	while (coders && ++i < count)
		free(coders[i]);
	free(coders);
	if (flags->heap) {
		if (flags->heap->queue)
			free(flags->heap->queue);
		if (flags->heap->going_out)
			free(flags->heap->going_out);
		if (flags->heap->staying)
			free(flags->heap->staying);
		if (flags->heap->mutex_ready)
			pthread_mutex_destroy(&flags->heap->mutex);
		if (flags->heap->cond_ready)
			pthread_cond_destroy(&flags->heap->cond);
		free(flags->heap);
	}
	pthread_mutex_destroy(&flags->print_mutex);
	pthread_mutex_destroy(&flags->dead_mutex);
	free(flags);
}
