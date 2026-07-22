/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 16:51:49 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/22 17:06:12 by iarrien-         ###   ########.fr       */
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

	i = -1;
	count = flags->number_of_coders;
	while (dongles && ++i < count)
	{
		if (!dongles[i])
			continue ;
		if (dongles[i]->cond_ready)
			pthread_cond_destroy(&dongles[i]->cond);
		if (dongles[i]->mutex_ready)
			pthread_mutex_destroy(&dongles[i]->mutex);
		free(dongles[i]->queue);
		free(dongles[i]);
	}
	free(dongles);
	i = -1;
	while (coders && ++i < count)
		free(coders[i]);
	free(coders);
	free(flags);
}
