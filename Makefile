# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/24 15:58:01 by iarrien-          #+#    #+#              #
#    Updated: 2026/08/10 17:41:59 by iarrien-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=codexion
CC=cc
CFLAGS=-Wall -Werror -Wextra -pthread -Isrc

ODIR=obj

SRC=main.c coders/coders_loop.c coders/coders_actions.c coders/coders_utils.c coders/check_simulation.c init/ft_free.c init/ft_init.c scheduler/heap_utils.c scheduler/heapify_utils.c scheduler/scheduler.c
VPATH=src
HEADER=coders.h
OBJ=$(addprefix $(ODIR)/,$(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

run: $(NAME)
	./$(NAME) $$(cat $(FILE))

$(ODIR)/%.o: %.c $(HEADER)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ODIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re