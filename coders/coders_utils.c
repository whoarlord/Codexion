/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:39:41 by iarrien-          #+#    #+#             */
/*   Updated: 2026/04/01 12:12:49 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

long long	calculate_time(long long time_to_calc)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000 + tv.tv_usec / 1000) - time_to_calc);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*result;
	size_t	i;

	if (!size || !nmemb)
		return (malloc(1));
	result = (char *)malloc(size * nmemb);
	if (!result)
		return (NULL);
	i = 0;
	while (i < nmemb * size)
	{
		result[i] = '\0';
		i++;
	}
	return ((void *)result);
}

void	update_dead(t_flags *flags)
{
	pthread_mutex_lock(&flags->dead_mutex);
	flags->is_dead = 1;
	pthread_mutex_unlock(&flags->dead_mutex);
}

int	check_dead(t_coder *coder)
{
	int	result;

	pthread_mutex_lock(&coder->flags->dead_mutex);
	result = coder->flags->is_dead;
	pthread_mutex_unlock(&coder->flags->dead_mutex);
	return (result);
}

void	wait_till_cooldown(t_coder *coder)
{
	if (coder->left->last_use == 0)
		coder->left->last_use -= coder->flags->dongle_cooldown;
	if (coder->right->last_use == 0)
		coder->right->last_use -= coder->flags->dongle_cooldown;
	while (coder->left->last_use
		+ coder->flags->dongle_cooldown
		>= calculate_time(coder->flags->start_time)
		|| coder->right->last_use
		+ coder->flags->dongle_cooldown
		>= calculate_time(coder->flags->start_time))
		;
}
