# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myagiz <myagiz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/15 17:15:39 by myagiz            #+#    #+#              #
#    Updated: 2024/02/15 17:15:41 by myagiz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
SRCS = ph_bonus.c ph_make_bonus.c ph_print_bonus.c ph_utils_bonus.c ph_sema_bonus.c ph_monitor_bonus.c
OBJS = $(SRCS:.c=.o)
INCS = ph_bonus.h
CC = cc
CFLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJS) $(INCS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $(SRCS)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
