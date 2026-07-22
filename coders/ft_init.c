/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 16:52:44 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/22 16:54:39 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	create_queue(t_dongle *dongle, int number_of_coders)
{
	dongle->queue = (t_queue *)ft_calloc(sizeof(t_queue), 1);
	if (!dongle->queue)
		return (1);
	if (number_of_coders == 1)
		dongle->queue->is_busy = 1;
	return (0);
}

int	init_dongle(t_dongle **dongles, int i, int number_of_coders)
{
	dongles[i] = (t_dongle *)ft_calloc(sizeof(t_dongle), 1);
	if (!dongles[i])
		return (1);
	dongles[i]->id = i;
	if (create_queue(dongles[i], number_of_coders))
		return (1);
	if (pthread_cond_init(&dongles[i]->cond, NULL) != 0)
		return (1);
	dongles[i]->cond_ready = 1;
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

int	create_cods_and_dongs(t_dongle **dongles, t_coder **coders, t_flags *flags)
{
	int	i;

	i = -1;
	while (++i < flags->number_of_coders)
		if (init_dongle(dongles, i, flags->number_of_coders))
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
