/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 16:52:44 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/10 15:52:26 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	init_dongle(t_dongle **dongles, int i)
{
	dongles[i] = (t_dongle *)ft_calloc(sizeof(t_dongle), 1);
	if (!dongles[i])
		return (1);
	dongles[i]->id = i;
	if (pthread_mutex_init(&dongles[i]->mutex, NULL) != 0)
		return (1);
	dongles[i]->mutex_ready = 1;
	return (0);
}

void	init_coder(t_coder **coders, t_dongle **dongles, int i, t_flags *flags)
{
	coders[i]->number = i + 1;
	coders[i]->left = dongles[i];
	if (i == flags->number_of_coders - 1)
		coders[i]->right = dongles[0];
	else
		coders[i]->right = dongles[i + 1];
	coders[i]->flags = flags;
}

int	create_heap(t_flags *flags)
{
	flags->heap = (t_heap *) ft_calloc(1, sizeof(t_heap));
	if (!flags->heap)
		return (1);
	flags->heap->queue = (t_request *) ft_calloc(flags->number_of_coders,
			sizeof(t_request));
	if (!flags->heap->queue)
		return (1);
	flags->heap->going_out = (t_request *) ft_calloc(flags->number_of_coders,
			sizeof(t_request));
	if (!flags->heap->going_out)
		return (1);
	flags->heap->staying = (t_request *) ft_calloc(flags->number_of_coders,
			sizeof(t_request));
	if (!flags->heap->staying)
		return (1);
	flags->heap->length = flags->number_of_coders;
	flags->heap->size = 0;
	if (pthread_mutex_init(&flags->heap->mutex, NULL) != 0)
		return (1);
	flags->heap->mutex_ready = 1;
	if (pthread_cond_init(&flags->heap->cond, NULL) != 0)
		return (1);
	flags->heap->cond_ready = 1;
	return (0);
}

int	create_cods_and_dongs(t_dongle **dongles, t_coder **coders, t_flags *flags)
{
	int	i;

	i = -1;
	while (++i < flags->number_of_coders)
		if (init_dongle(dongles, i))
			return (1);
	i = -1;
	while (++i < flags->number_of_coders)
	{
		coders[i] = (t_coder *)ft_calloc(sizeof(t_coder), 1);
		if (!coders[i])
			return (1);
		init_coder(coders, dongles, i, flags);
	}
	return (0);
}
