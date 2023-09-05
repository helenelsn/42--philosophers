# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Helene <Helene@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/13 19:15:07 by Helene            #+#    #+#              #
#    Updated: 2023/09/05 13:25:07 by Helene           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
NAME_BONUS = philo_bonus
CC = cc
CFLAGS = -g3  -fsanitize=address -g # -g pour afficher les numéros de lignes qui ont produit l'erreur
T_FLAGS = -fsanitize=thread -g
# -Wall -Werror -Wextra 
RM = rm -f

INC = philo.h
INC_DIR = ./includes/
INC_FILES = $(addprefix $(INC_DIR), $(INC))

SRC_DIR = ./sources/
SRC = 	main.c \
		routine.c \
		monitoring.c \
		initialise.c \
		destroy.c \
		ft_usleep.c \
		ft_fprintf.c

OBJ_DIR = ./objects/
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

export C_INCLUDE_PATH = $(INC_DIR)
# pas besoin d'export LIBRARY_PATH car n'utilise pas de librairie(s) ici

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(INC_FILES)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS_DIR) -pthread

clean :
	$(RM) -r $(OBJ_DIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY :
	all clean fclean re 