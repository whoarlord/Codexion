/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:02:07 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/16 16:16:08 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

int	fill_flags(int i, char *argv[], t_flags *flags)
{
	if (i == 1)
		flags->number_of_coders = atoi(argv[i]);
	else if (i == 2)
		flags->time_to_burnout = atoi(argv[i]);
	else if (i == 3)
		flags->time_to_compile = atoi(argv[i]);
	else if (i == 4)
		flags->time_to_debug = atoi(argv[i]);
	else if (i == 5)
		flags->time_to_refactor = atoi(argv[i]);
	else if (i == 6)
		flags->number_of_compiles_required = atoi(argv[i]);
	else if (i == 7)
		flags->dongle_cooldown = atoi(argv[i]);
	else if (i == 8)
	{
		if (!strcmp(argv[i], "fifo"))
			flags->scheduler = fifo;
		else if (!strcmp(argv[i], "edf"))
			flags->scheduler = edf;
		else
			return (1);
	}
	return (0);
}

void	free_queue(t_queue *queue)
{
	free(queue);
}

void	ft_free_cods_and_dongs(t_dongle **dongles,
	t_coder **coders, t_flags *flags)
{
	int	i;

	i = 0;
	while (i < flags->number_of_coders)
	{
		free(dongles[i]->queue);
		free(dongles[i]);
		free(coders[i]);
		i++;
	}
	free(dongles);
	free(coders);
	free(flags);
}

void	create_queue(t_dongle *dongle, int number_of_coders)
{
	dongle->queue = (t_queue *)ft_calloc(sizeof(t_queue), 1);
	if (number_of_coders == 1)
		dongle->queue->is_busy = 1;
	pthread_cond_init(&dongle->queue->cond, NULL);
	pthread_mutex_init(&dongle->queue->mutex, NULL);
}

void	create_cods_and_dongs(t_dongle **dongles, t_coder **coders,
		t_flags *flags)
{
	int	i;

	i = 0;
	while (i < flags->number_of_coders)
	{
		dongles[i] = (t_dongle *)ft_calloc(sizeof(t_dongle), 1);
		dongles[i]->id = i;
		create_queue(dongles[i], flags->number_of_coders);
		pthread_mutex_init(&dongles[i]->mutex, NULL);
		i++;
	}
	i = 0;
	while (i < flags->number_of_coders)
	{
		coders[i] = (t_coder *)ft_calloc(sizeof(t_coder), 1);
		coders[i]->number = i + 1;
		coders[i]->last_compile = 0;
		coders[i]->left = dongles[i];
		if (i == flags->number_of_coders - 1)
			coders[i]->right = dongles[0];
		else
			coders[i]->right = dongles[i + 1];
		coders[i]->flags = flags;
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_flags		*flags;
	int			i;
	t_dongle	**dongles;
	t_coder		**coders;

	flags = (t_flags *)ft_calloc(sizeof(t_flags), 1);
	pthread_mutex_init(&flags->print_mutex, NULL);
	pthread_mutex_init(&flags->dead_mutex, NULL);
	if (argc != 9)
		return (printf("Incorrect number of parameters\n"), free(flags), 1);
	i = 1;
	while (i < argc)
	{
		if (fill_flags(i, argv, flags))
			return ((printf("Incorrect inputs\n"), free(flags), 1));
		i++;
	}
	flags->start_time = calculate_time(0);
	dongles = (t_dongle **)ft_calloc(sizeof(t_dongle *),
			flags->number_of_coders);
	coders = (t_coder **)ft_calloc(sizeof(t_coder *), flags->number_of_coders);
	create_cods_and_dongs(dongles, coders, flags);
	initialize_threads(coders, flags);
	return (ft_free_cods_and_dongs(dongles, coders, flags), 0);
}
