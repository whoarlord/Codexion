/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/26 13:39:41 by iarrien-          #+#    #+#             */
/*   Updated: 2026/03/26 15:41:09 by iarrien-         ###   ########.fr       */
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
