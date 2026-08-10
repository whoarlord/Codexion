/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_simulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/10 17:24:16 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/10 17:44:03 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	create_sched_cods(int *created, t_flags *flags,
						pthread_t *scheduler, t_coder **coders)
{
	int	i;

	i = 0;
	if (*created > 0)
	{
		if (pthread_create(scheduler, NULL, scheduler_loop, flags) != 0)
			update_dead(flags);
		else
			*created = *created + 1;
	}
	if (*created > 1)
	{
		while (i < flags->number_of_coders)
		{
			if (pthread_create(&coders[i]->thread, NULL,
					coders_loop, coders[i]) != 0)
			{
				update_dead(flags);
				break ;
			}
			*created = *created + 1;
			i++;
		}
	}
	pthread_cond_broadcast(&flags->heap->cond);
}
