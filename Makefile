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

### VARIABLES ###

NAME = minishell
HEADER = ./include/
CC = gcc -I ~/.brew/opt/readline/include -I /usr/local/opt/readline/include
CRL = -L /usr/local/opt/readline/lib -I /usr/local/opt/readline/include	#Link the program with th Readline library
CFLAGS += -Wall -Werror -Wextra -I $(HEADER)
OBJF = .cache_exists	#Needed to create obj/ directory
.DEFAULT_GOAL := all

# Directories
MAIN_DIR = src/
BUILTIN_DIR = src/builtins/
EXECUTOR_DIR = src/executor/
PARSING_DIR = src/parsing/
PIPEX_DIR = src/pipex/
ENV_DIR = src/environment/
UTILS_DIR = src/utils/
OBJ_DIR = obj/
update_flag := false

# Source Files
MAIN_FILES = main.c
BUILTIN_FILES = builtins.c cd.c cd_utils_1.c cd_utils_2.c echo.c exit.c env.c export_1.c \
					export_2.c unset.c builtins_utils_1.c builtins_utils_2.c
EXECUTOR_FILES = executor.c what_to_do.c executor_utils.c solo_executor.c directory.c path.c
PARSING_FILES = delimiters_parsing.c parsing_init.c parsing_heredoc.c parsing_error_1.c \
					parsing_error_2.c parsing_utils_1.c parsing_utils_2.c parsing_var_hdc.c \
					make_list_command_init.c make_list_command_1.c make_list_command_2.c
PIPEX_FILES = heredoc_1.c heredoc_2.c heredoc_var_1.c heredoc_var_2.c pipex.c pipex_2.c \
				pipex_error.c pipex_utils.c redirection_1.c redirection_2.c
ENV_FILES = dollar_1.c dollar_2.c underscore.c shlvl.c
UTILS_FILES = utils_1.c utils_2.c history.c signals_1.c signals_2.c error.c 

# Object Files
MAIN_OBJ = $(addprefix $(OBJ_DIR), $(MAIN_FILES:.c=.o))
BUILTIN_OBJ = $(addprefix $(OBJ_DIR), $(BUILTIN_FILES:.c=.o))
EXECUTOR_OBJ = $(addprefix $(OBJ_DIR), $(EXECUTOR_FILES:.c=.o))
PARSING_OBJ = $(addprefix $(OBJ_DIR), $(PARSING_FILES:.c=.o))
PIPEX_OBJ = $(addprefix $(OBJ_DIR), $(PIPEX_FILES:.c=.o))
ENV_OBJ = $(addprefix $(OBJ_DIR), $(ENV_FILES:.c=.o))
UTILS_OBJ = $(addprefix $(OBJ_DIR), $(UTILS_FILES:.c=.o))

### COMPILATION ###

$(OBJF):
	@mkdir -p $(OBJ_DIR)

# Creating object files (.o) from source files (.c) inside the OBJ_DIR directory.
# The option -o indicates the name of the outpout, the option -c indicates the source that is used.

$(OBJ_DIR)%.o : $(MAIN_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)%.o : $(BUILTIN_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)%.o : $(EXECUTOR_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)%.o : $(PARSING_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)%.o : $(PIPEX_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)%.o : $(ENV_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)%.o : $(UTILS_DIR)%.c $(HEADER) | $(OBJF)
	@printf "$(YELLOW)Generating minishell objects... %-33.33s\r" $@
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# Compiling all objets files (.o) to a file "NAME" :
$(NAME) : $(MAIN_OBJ) $(BUILTIN_OBJ) $(EXECUTOR_OBJ) $(PARSING_OBJ) $(PIPEX_OBJ) $(ENV_OBJ) $(UTILS_OBJ)
	@$(MAKE) -C ./libft
	@$(CC) $(CRL) $(CFLAGS) -o $@ $^ ./libft/libft.a -lreadline
	@echo " "
	@echo "$(GREEN)-> $(NAME) compiled!$(DEF_COLOR)"
	@echo " "
	@$(eval update_flag := true)

### MEMO ###
# 	Spécial variables :
#		$@	Represents the target file that is being generated by the rule
#		$<	Represents the first dependency of the rule
#		$^	Represents all the dependencies of the rule, separated by spaces.
#		$?	Represents a list of dependencies that are newer than the target file.
#		$*	Represents the stem of the target filename (without the extension)

### RULES ##

# Flag to check if 'clean' is executed alone or as a dependency of 'fclean'
fclean_flag := false
do_fclean:
	@$(eval fclean_flag := true)

nothing_to_be_done:
	@if [ "$(update_flag)" = "false" ]; then \
		echo "$(GREEN)-> make: Nothing to be done for 'all'.$(DEF_COLOR)"; \
	fi

all : $(NAME) nothing_to_be_done

clean:
	@echo "$(YELLOW)Cleaning $(NAME)... $(DEF_COLOR)"
	@$(MAKE) -C ./libft fclean
	@rm -rf $(OBJ_DIR)
	@echo " "
	@if [ "$(fclean_flag)" = "false" ]; then \
		echo "$(GREEN)-> $(NAME) object files cleaned!$(DEF_COLOR)"; \
    fi
	@if [ "$(fclean_flag)" = "true" ]; then \
		echo "$(GREEN)-> $(NAME) object files & executables cleaned!$(DEF_COLOR)"; \
    fi
	@echo " "

fclean: do_fclean clean
	@rm -f $(NAME)

re:	fclean all
	@echo "$(BLUE)-> Cleaned and rebuilt everything for $(NAME)!$(DEF_COLOR)"

# Phony targets are used to differenciate makefile rules from system files.
.PHONY: all clean fclean re