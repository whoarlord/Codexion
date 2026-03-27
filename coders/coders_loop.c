/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:21:58 by iarrien-          #+#    #+#             */
/*   Updated: 2026/03/26 17:09:06 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


int	monitor_verifications(t_coder **coders, int get_out, int *i)
{
	t_flags	*flags;

	flags = coders[0]->flags;
	while (*i < flags->number_of_coders)
	{
		if (coders[*i]->compile_count
			>= flags->number_of_compiles_required)
			get_out++;
		if (flags->time_to_burnout + coders[*i]->last_compile
			<= calculate_time(flags->start_time))
		{
			get_out = -1;
			break ;
		}
		pthread_cond_broadcast(&flags->finish_cond);
		*i = *i + 1;
	}
	if (*i == flags->number_of_coders)
		*i = *i - 1;
	return (get_out);
}


void	*monitor_loop(void *coders_pointer)
{
	t_coder	**coders;
	int		i;
	int		get_out;

	coders = (t_coder **)coders_pointer;
	while (1)
	{
		get_out = 0;
		i = 0;
		get_out = monitor_verifications(coders, get_out, &i);
		if (get_out == coders[i]->flags->number_of_coders)
		{
			printf("Finished\n");
			break ;
		}
		else if (get_out == -1)
		{
			print_action(coders[i]->number,
				coders[i]->flags->start_time, "burned out");
			break ;
		}
	}
	return (NULL);
}

void	*coders_loop(void *coder_pointer)
{
	t_coder	*coder;

	coder = (t_coder *)coder_pointer;
	while (1)
	{
		fifo_queue(coder);
		pthread_mutex_lock(&coder->right->mutex);
		pthread_mutex_lock(&coder->flags->print_mutex);
		take_dongle(coder);
		pthread_mutex_lock(&coder->left->mutex);
		take_dongle(coder);
		compile(coder);
		pthread_mutex_unlock(&coder->right->mutex);
		pthread_mutex_unlock(&coder->left->mutex);
		free_coder_from_queue(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

void	initialize_threads(t_coder **coders, t_flags *flags)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	pthread_create(&monitor, NULL, monitor_loop, coders);
	while (i < flags->number_of_coders)
	{
		pthread_create(&coders[i]->thread, NULL, coders_loop, coders[i]);
		i++;
	}
	pthread_join(monitor, NULL);
}
