### COLORS ###
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;33m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

SRC = ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isdigit.c ft_islower.c ft_isupper.c ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_split.c ft_strdup.c ft_strdup.c ft_strjoin.c ft_strlen.c ft_strncmp.c ft_strnstr.c

CC = gcc

CC_FLAGS = -Wall -Werror -Wextra

NAME = libft.a

OBJ = $(SRC:.c=.o)

all : $(NAME)

$(NAME) :
	@echo "$(YELLOW)Compiling libft... $(DEF_COLOR)"
	@$(CC) $(CC_FLAGS) -c $(SRC)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)
	@echo " "
	@echo "$(CYAN)libft compiled!$(DEF_COLOR)"


clean :
	@echo "$(YELLOW)Cleaning libft... $(DEF_COLOR)"
	@rm -f $(OBJ)
	@rm -f $(BONUS_OBJ)
	@echo "$(BLUE)libft object files cleaned!$(DEF_COLOR)"

fclean : clean
	@rm -f $(NAME)
	@echo "$(BLUE)libft archive cleaned!$(DEF_COLOR)"

re : fclean all

.PHONY: clean fclean
