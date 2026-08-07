/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:21:58 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/07 17:04:19 by iarrien-         ###   ########.fr       */
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
		*i = *i + 1;
	}
	if (*i == flags->number_of_coders)
		*i = *i - 1;
	return (get_out);
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
	int		get_out;

	coders = (t_coder **)coders_pointer;
	while (1)
	{
		get_out = 0;
		i = 0;
		get_out = monitor_verifications(coders, get_out, &i);
		if (get_out == coders[i]->flags->number_of_coders)
		{
			finish_state(coders[i], "Finished");
			break ;
		}
		else if (get_out == -1)
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
	functions[0] = send_request;
	functions[1] = take_and_compile;
	functions[2] = debug;
	functions[3] = refactor;
	i = 0;
	while (!check_dead(coder->flags))
	{
		if (functions[i % 4](coder))
			break ;
		i++;
	}
	printf("coder: %d, out\n", coder->number);
	return (NULL);
}

void	initialize_threads(t_coder **coders, t_flags *flags)
{
	int			i;
	pthread_t	monitor;
	pthread_t	scheduler;

	i = 0;
	pthread_create(&monitor, NULL, monitor_loop, coders);
	pthread_create(&scheduler, NULL, scheduler_loop, flags);
	while (i < flags->number_of_coders)
	{
		pthread_create(&coders[i]->thread, NULL, coders_loop, coders[i]);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 0;
	while (i < flags->number_of_coders)
	{
		pthread_cond_broadcast(&flags->heap->cond);
		pthread_join(coders[i]->thread, NULL);
		i++;
	}
}
