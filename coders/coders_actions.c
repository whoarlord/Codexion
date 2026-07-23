/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:17:00 by iarrien-          #+#    #+#             */
/*   Updated: 2026/07/23 16:52:50 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coders.h"

void	print_action(int number, long long start_time, char *src)
{
	printf("%lld %d %s\n", calculate_time(start_time), number, src);
}

int	take_and_compile(t_coder *coder)
{
	if (check_dead(coder))
		return (pthread_mutex_unlock(&coder->right->mutex),
			pthread_mutex_unlock(&coder->flags->print_mutex),
			update_coders_queue(coder), 1);
	pthread_mutex_lock(&coder->flags->print_mutex);
	pthread_mutex_lock(&coder->right->mutex);
	if (check_dead(coder))
		return (pthread_mutex_unlock(&coder->right->mutex),
			pthread_mutex_unlock(&coder->flags->print_mutex),
			update_coders_queue(coder), 1);
	coder->last_compile = calculate_time(coder->flags->start_time);
	print_action(coder->number, coder->flags->start_time, "has taken a dongle");
	pthread_mutex_lock(&coder->left->mutex);
	print_action(coder->number, coder->flags->start_time, "has taken a dongle");
	coder->last_compile = calculate_time(coder->flags->start_time);
	print_action(coder->number, coder->flags->start_time, "is compiling");
	pthread_mutex_unlock(&coder->flags->print_mutex);
	coder->next_event_time = calculate_time(coder->flags->start_time) + (long long) (coder->flags->time_to_compile);
	while (coder->next_event_time > calculate_time(coder->flags->start_time)) {
		if (check_dead(coder))
			return (pthread_mutex_unlock(&coder->right->mutex),
				pthread_mutex_unlock(&coder->left->mutex),
				update_coders_queue(coder), 1);
		usleep(100);
	}
	coder->right->last_use = calculate_time(coder->flags->start_time);
	coder->left->last_use = calculate_time(coder->flags->start_time);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->right->mutex);
	pthread_mutex_unlock(&coder->left->mutex);
	update_coders_queue(coder);
	return (0);
}

int	debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->flags->print_mutex);
	if (check_dead(coder))
		return (pthread_mutex_unlock(&coder->flags->print_mutex), 1);
	print_action(coder->number, coder->flags->start_time, "is debugging");
	pthread_mutex_unlock(&coder->flags->print_mutex);
	coder->next_event_time += (long long) (coder->flags->time_to_debug);
	while (coder->next_event_time > calculate_time(coder->flags->start_time)) {
		if (check_dead(coder))
			return (1);
		usleep(100);
	}
	return (0);
}

int	refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->flags->print_mutex);
	if (check_dead(coder))
		return (pthread_mutex_unlock(&coder->flags->print_mutex), 1);
	print_action(coder->number, coder->flags->start_time, "is refactoring");
	pthread_mutex_unlock(&coder->flags->print_mutex);
	coder->next_event_time += (long long) (coder->flags->time_to_refactor);
	while (coder->next_event_time > calculate_time(coder->flags->start_time)) {
		if (check_dead(coder))
			return (1);
		usleep(100);
	}
	return (0);
}
