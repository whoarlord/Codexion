/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:21:58 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/14 18:25:46 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"


int	monitor_verifications(t_coder **coders, int mal, int *i)
{
	t_flags	*flags;

	flags = coders[0]->flags;
	while (*i < flags->number_of_coders)
	{
		if (coders[*i]->compile_count
			>= flags->number_of_compiles_required)
			mal++;
		if (flags->time_to_burnout + coders[*i]->last_compile
			<= calculate_time(flags->start_time))
		{
			mal = -1;
			break ;
		}
		*i = *i + 1;
	}
	if (*i == flags->number_of_coders)
		*i = *i - 1;
	return (mal);
}


static void	finish_state(t_coder *coder, char *str)
{
	t_flags	*flags;

	flags = coder->flags;
	update_dead(flags);
	pthread_mutex_lock(&flags->print_mutex);
	if (!strcmp(str, "burned out"))
		print_action(coder->number, flags->start_time, str);
	else
		printf("%lld %s\n", calculate_time(flags->start_time), str);
	pthread_mutex_unlock(&flags->print_mutex);
}


void	*monitor_loop(void *coders_pointer)
{
	t_coder	**coders;
	int		i;
	int		mal;

	coders = (t_coder **)coders_pointer;
	while (1)
	{
		mal = 0;
		i = 0;
		mal = monitor_verifications(coders, mal, &i);
		if (mal == coders[i]->flags->number_of_coders)
		{
			finish_state(coders[i], "Finished");
			break ;
		}
		else if (mal == -1)
		{
			finish_state(coders[i], "burned out");
			break ;
		}
	}
	return (NULL);
}

void	*coders_loop(void *coder_pointer)
{
	t_coder		*coder;
	t_functions	functions[4];
	int			i;

	coder = (t_coder *)coder_pointer;
	functions[0] = fifo_queue;
	if (coder->flags->scheduler == edf)
		functions[0] = edf_queue;
	functions[1] = take_and_compile;
	functions[2] = debug;
	functions[3] = refactor;
	i = 0;
	while (!check_dead(coder))
	{
		if (functions[i % 4](coder))
			break ;
		i++;
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
	i = 0;
	while (i < flags->number_of_coders)
	{
		pthread_cond_broadcast(&coders[i]->left->queue->cond);
		pthread_cond_broadcast(&coders[i]->right->queue->cond);
		pthread_join(coders[i]->thread, NULL);
		i++;
	}
}
