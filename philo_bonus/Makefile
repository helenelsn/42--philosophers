# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Helene <Helene@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/09 20:42:20 by Helene            #+#    #+#              #
#    Updated: 2023/09/11 21:30:27 by Helene           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
CC = cc
CFLAGS = -I -g3 -MMD #-fsanitize=address -g # -g pour afficher les numéros de lignes qui ont produit l'erreur
T_FLAGS = -fsanitize=thread -g 
# -Wall -Werror -Wextra 
RM = rm -f

INC_DIR = ./includes/

SRC_DIR = ./sources/
SRC = 	main.c \
		initialise.c \
		main_monitoring.c \
		routine.c \
		destroy.c \
		utils.c \
		ft_atoi.c 

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