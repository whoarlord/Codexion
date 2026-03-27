/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:17:00 by iarrien-          #+#    #+#             */
/*   Updated: 2026/03/26 17:07:36 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	print_action(int number, long long start_time, char *src)
{
	printf("%lld %d %s\n", calculate_time(start_time), number, src);
}

void	take_dongle(t_coder *coder)
{
	print_action(coder->number, coder->flags->start_time, "has taken a dongle");
}

void	compile(t_coder *coder)
{
	coder->last_compile = calculate_time(coder->flags->start_time);
	print_action(coder->number, coder->flags->start_time, "is compiling");
	pthread_mutex_unlock(&coder->flags->print_mutex);
	usleep(coder->flags->time_to_compile * 1000);
	coder->last_compile = calculate_time(coder->flags->start_time);
	coder->right->last_use = calculate_time(coder->flags->start_time);
	coder->left->last_use = calculate_time(coder->flags->start_time);
	coder->compile_count++;
	pthread_mutex_lock(&coder->flags->print_mutex);
	pthread_cond_wait(&coder->flags->finish_cond, &coder->flags->print_mutex);
	pthread_mutex_unlock(&coder->flags->print_mutex);
}

void	debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->flags->print_mutex);
	print_action(coder->number, coder->flags->start_time, "is debugging");
	pthread_mutex_unlock(&coder->flags->print_mutex);
	usleep(coder->flags->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->flags->print_mutex);
	print_action(coder->number, coder->flags->start_time, "is refactoring");
	pthread_mutex_unlock(&coder->flags->print_mutex);
	usleep(coder->flags->time_to_refactor * 1000);
}
