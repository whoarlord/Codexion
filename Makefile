# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iarrien- <iarrien-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/24 15:58:01 by iarrien-          #+#    #+#              #
#    Updated: 2026/07/22 17:16:32 by iarrien-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=codexion
CC=cc
CFLAGS=-Wall -Werror -Wextra -pthread

ODIR=obj

SRC=main.c coders_loop.c coders_actions.c coders_utils.c fifo.c check_before.c queue_utils.c edf.c ft_free.c ft_init.c
SDIR=coders
HEADER=coders.h
OBJ=$(addprefix $(ODIR)/,$(SRC:.c=.o))

all: $(NAME)

$(NAME): $(ODIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

run: $(NAME)
	./$(NAME) $$(cat $(FILE))

$(ODIR)/%.o: $(SDIR)/%.c $(SDIR)/$(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir $(ODIR)

clean:
	rm -rf $(ODIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re