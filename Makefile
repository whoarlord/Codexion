# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/24 15:58:01 by iarrien-          #+#    #+#              #
#    Updated: 2026/03/31 16:08:49 by iarrien-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=codexion
CC=cc
CFLAGS=-Wall -Werror -Wextra -pthread -g3 -fsanitize=address

SRC=main.c coders_loop.c coders_actions.c coders_utils.c fifo.c check_before.c
SDIR=coders
HEADER=coders.h
OBJ=$(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: $(SDIR)/%.c $(SDIR)/$(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re