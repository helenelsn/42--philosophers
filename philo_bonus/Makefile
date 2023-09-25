# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hlesny <hlesny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/09 20:42:20 by Helene            #+#    #+#              #
#    Updated: 2023/09/25 16:34:25 by hlesny           ###   ########.fr        #
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
		initialise_utils.c \
		parsing.c \
		main_monitoring.c \
		routine.c \
		philo_states.c \
		death_while_waiting.c \
		destroy.c \
		utils.c \
		sem_death_protection.c \
		ft_atoi.c \
		ft_itoa.c \
		ft_calloc.c

OBJ_DIR = ./objects/
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEP_OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.d))

#export C_INCLUDE_PATH = $(INC_DIR):$(INC_BONUS_DIR)

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