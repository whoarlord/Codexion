/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:02:53 by iarrien-          #+#    #+#             */
/*   Updated: 2026/08/07 16:11:47 by iarrien-         ###   ########.fr       */
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

typedef struct s_coder	t_coder;

typedef enum e_scheduler
{
	fifo,
	edf
}					t_scheduler;

typedef struct s_request
{
	t_coder		*coder;
	long long 	arrival;
	long long	time_to_burnout;
}				t_request;

typedef struct s_heap
{
	t_request		*queue;
	t_request		*staying;
	t_request		*going_out;
	int				size;
	int				length;
	pthread_mutex_t	mutex;
	int				mutex_ready;
	pthread_cond_t	cond;
	int				cond_ready;
	int				last_arrival;
	int				stop_popping;
}					t_heap;

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
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	dead_mutex;
	int				is_dead;
	t_heap			*heap;
}					t_flags;

typedef struct s_dongle
{
	int				id;
	long long		last_use;
	int				is_busy;
	pthread_mutex_t	mutex;
	int				mutex_ready;
}					t_dongle;

typedef struct s_coder
{
	int				number;
	int				last_compile;
	int				compile_count;
	int				next_event_time;
	t_dongle		*left;
	t_dongle		*right;
	pthread_t		thread;
	t_flags			*flags;
	int				go_out;
}					t_coder;

typedef int	(*t_functions) (t_coder *coder);

void	init_coder(t_coder **coders, t_dongle **dongles, int i, t_flags *flags);
int	create_cods_and_dongs(t_dongle **dongles, t_coder **coders, t_flags *flags);
void	free_flags(t_flags *flags);
void	ft_free_everything(t_dongle **dongles, t_coder **coders, t_flags *flags);


void				*coders_loop(void *coder);
void				initialize_threads(t_coder **coders, t_flags *flags);
void				print_action(int number, long long start_time, char *src);
int					take_and_compile(t_coder *coder);
int					debug(t_coder *coder);
int					refactor(t_coder *coder);

long long			calculate_time(long long time_to_calc);
void				*ft_calloc(size_t nmemb, size_t size);
void				wait_till_cooldown(t_coder *coder);
int					check_dead(t_flags *flags);
void				update_dead(t_flags *flags);

void		heapify_down(t_flags *flags);
void		heapify_up(t_flags *flags);
int		create_heap(t_flags *flags);
void		heap_push(t_flags *flags, t_coder	*coder);
t_request	heap_pop(t_flags *flags);
int		send_request(t_coder *coder);
void	*scheduler_loop(void *flags);
void	print_queue(t_heap *heap);
void	clear_heap(t_flags *flags);
#endif
