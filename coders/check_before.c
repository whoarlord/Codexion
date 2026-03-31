/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_before.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 16:07:28 by iarrien-          #+#    #+#             */
/*   Updated: 2026/03/31 16:27:54 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

static int	check_before_equal(int *free_dongles, t_coder *coder, int i)
{
	return (free_dongles[i] == 1
		&& (i == coder->left->id
			|| i == coder->right->id));

}

static int	check_before_different(int *free_dongles, int i)
{
	return (free_dongles[i] == 0);

}

int	check_before_coders(int *coders, int *free_dongles, t_coder *coder)
{
	int	i;
	int	n;

	i = 0;
	n = coder->flags->number_of_coders;
	while (i < n)
	{
		if (coders[i] == coder->number)
			break ;
		else if ((check_before_equal(free_dongles, coder, coders[i] - 1)
				|| check_before_equal(free_dongles, coder, coders[i] % n))
			|| (check_before_different(free_dongles, coders[i] - 1)
				&& check_before_different(free_dongles, coders[i] % n)))
			return (1);
		i++;
	}
	return (0);
}
