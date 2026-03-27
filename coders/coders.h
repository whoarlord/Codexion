/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:02:53 by iarrien-          #+#    #+#             */
/*   Updated: 2026/03/26 17:10:01 by iarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODERS_H
# define CODERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum e_scheduler
{
	fifo,
	edf
}					t_scheduler;
typedef struct s_queue
{
	int				*coders;
	int				*free_dongles;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}					t_queue;

typedef struct s_flags
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	long long		start_time;
	t_scheduler		scheduler;
	t_queue			*queue;
	pthread_mutex_t	print_mutex;
	pthread_cond_t	finish_cond;
}					t_flags;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				id;
	long long		last_use;
}					t_dongle;

typedef struct s_coder
{
	int				number;
	int				last_compile;
	int				compile_count;
	t_dongle		*left;
	t_dongle		*right;
	pthread_t		thread;
	t_flags			*flags;
}					t_coder;

void				*coders_loop(void *coder);
void				initialize_threads(t_coder **coders, t_flags *flags);
long long			calculate_time(long long time_to_calc);
void				print_action(int number, long long start_time, char *src);
void				take_dongle(t_coder *coder);
void				compile(t_coder *coder);
void				debug(t_coder *coder);
void				refactor(t_coder *coder);
void				*ft_calloc(size_t nmemb, size_t size);

void				free_coder_from_queue(t_coder *coder);
void				fifo_queue(t_coder *coder);
#endif
