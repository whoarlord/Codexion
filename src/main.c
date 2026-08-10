/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:02:07 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/10 17:16:24 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"
#include <sys/resource.h>

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

static int	initialize_simulation(t_flags *flags)
{
	t_dongle	**dongles;
	t_coder		**coders;

	dongles = (t_dongle **)ft_calloc(sizeof(t_dongle *),
			flags->number_of_coders);
	if (!dongles)
		return (free_flags(flags), 1);
	coders = (t_coder **)ft_calloc(sizeof(t_coder *), flags->number_of_coders);
	if (!coders)
		return (free_flags(flags), free(dongles), 1);
	if (create_cods_and_dongs(dongles, coders, flags))
		return (ft_free_everything(dongles, coders, flags), 1);
	if (create_heap(flags))
		return (ft_free_everything(dongles, coders, flags), 1);
	initialize_threads(coders, flags);
	return (ft_free_everything(dongles, coders, flags), 0);
}

static int	check_input(t_flags *flags)
{
	if (flags->number_of_coders <= 0)
		return (printf("Incorrect number of coders: %d\n",
				flags->number_of_coders), 1);
	else if (flags->time_to_burnout <= 0)
		return (printf("Incorrect time to burnout: %d\n",
				flags->time_to_burnout), 1);
	else if (flags->time_to_compile <= 0)
		return (printf("Incorrect time to compile: %d\n",
				flags->time_to_compile), 1);
	else if (flags->time_to_debug <= 0)
		return (printf("Incorrect time to debug: %d\n",
				flags->time_to_debug), 1);
	else if (flags->time_to_refactor <= 0)
		return (printf("Incorrect time to refactor: %d\n",
				flags->time_to_refactor), 1);
	else if (flags->number_of_compiles_required <= 0)
		return (printf("Incorrect number of compiles: %d\n",
				flags->number_of_compiles_required), 1);
	else if (flags->dongle_cooldown < 0)
		return (printf("Incorrect dongle cooldown: %d\n",
				flags->dongle_cooldown), 1);
	return (0);
}

static int	non_number_chars(char *str)
{
	int	len;
	int	i;

	len = strlen(str);
	i = 0;
	while (i < len)
	{
		if (str[i] < 48 || str[i] > 57)
			return (1);
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_flags		*flags;
	int			i;

	flags = (t_flags *)ft_calloc(sizeof(t_flags), 1);
	if (!flags)
		exit(1);
	if (pthread_mutex_init(&flags->print_mutex, NULL) != 0)
		return (free(flags), 1);
	if (pthread_mutex_init(&flags->dead_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&flags->print_mutex), free(flags), 1);
	if (argc != 9)
		return (printf("Incorrect number of parameters\n"), free(flags), 1);
	i = 1;
	while (i < argc)
	{
		if ((i >= 1 && i <= 7 && non_number_chars(argv[i]))
			|| fill_flags(i, argv, flags))
			return ((printf("Incorrect inputs\n"), free(flags), 1));
		i++;
	}
	if (check_input(flags))
		return (free(flags), 1);
	flags->start_time = calculate_time(0);
	return (initialize_simulation(flags));
}
