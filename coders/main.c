/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:02:07 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/22 17:20:28 by iarrien-         ###   ########.fr       */
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

int	main(int argc, char *argv[])
{
	t_flags		*flags;
	int			i;
	t_dongle	**dongles;
	t_coder		**coders;
	/*struct rlimit lim;

	 lim.rlim_cur = 100 * 1024;
    lim.rlim_max = 100 * 1024;

	if (setrlimit(RLIMIT_AS, &lim) != 0) {
		printf("error inicio\n");
        perror("setrlimit");
        return 1;
    } */

	flags = (t_flags *)ft_calloc(sizeof(t_flags), 1);
	if (!flags)
		exit(1);
	if (pthread_mutex_init(&flags->print_mutex, NULL) != 0)
		return (free(flags), 1);
	printf("here\n");
	if (pthread_mutex_init(&flags->dead_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&flags->print_mutex), free(flags), 1);
	if (argc != 9)
		return (printf("Incorrect number of parameters\n"), free(flags), 1);
	i = 1;
	while (i < argc)
	{
		if (fill_flags(i, argv, flags))
			return ((printf("Incorrect inputs\n"), free(flags), 1));
		i++;
	}
	printf("here\n");
	flags->start_time = calculate_time(0);
	dongles = (t_dongle **)ft_calloc(sizeof(t_dongle *),
			flags->number_of_coders);
	if (!dongles)
		return (free_flags(flags), 1);
	coders = (t_coder **)ft_calloc(sizeof(t_coder *), flags->number_of_coders);
	if (!dongles)
		return (free_flags(flags), free(dongles), 1);
	if (create_cods_and_dongs(dongles, coders, flags))
		return (ft_free_everything(dongles, coders, flags), 1);
	initialize_threads(coders, flags);
	return (ft_free_everything(dongles, coders, flags), 0);
}
