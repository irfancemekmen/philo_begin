SRC = philo_operations.c main.c philo_operations2.c\
		utils.c one_philo.c\

NAME = philo
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -pthread
RM = rm -f

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
clean:
	$(RM) $(OBJ)
fclean: clean
	$(RM) $(NAME) $(OBJ)
re: fclean all
.PHONY: all clean fclean re
