# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Helene <Helene@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/09 20:38:24 by Helene            #+#    #+#              #
#    Updated: 2023/09/11 17:42:30 by Helene           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -I -g3 -MMD #-fsanitize=address -g # -g pour afficher les numéros de lignes qui ont produit l'erreur
T_FLAGS = -fsanitize=thread -g 
# -Wall -Werror -Wextra 
RM = rm -f

INC_DIR = ./includes/

SRC_DIR = ./sources/
SRC = 	main.c \
		routine.c \
		eating.c \
		monitoring.c \
		initialise.c \
		destroy.c \
		ft_atoi.c \
		utils.c

OBJ_DIR = ./objects/
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

DEP_OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.d))

#export C_INCLUDE_PATH = $(INC_DIR):$(INC_BONUS_DIR)
# pas besoin d'export LIBRARY_PATH car n'utilise pas de librairie(s) ici

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCS_DIR) -pthread

-include $(DEP_OBJS)

clean :
	$(RM) -r $(OBJ_DIR)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY :
	all clean fclean re 