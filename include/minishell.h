/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 15:26:33 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 01:27:27 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define READ 0
# define WRITE 1
# define STDIN 0
# define STDOUT 1
# define CHILD 0
# define CTRL_C 1
# define CTRL_D 2
# define CTRL_C_HEREDOC 3

typedef struct s_heredoc
{
	char				*line;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_struct
{
	char				*content;
	int					is_pipe;
	int					is_redirect_in;
	int					is_redirect_out;
	int					is_append;
	int					is_heredoc;
	int					is_new_line;
	int					is_var;
	int					is_empty_var;
	int					expand;
	int					hdc_quotes;
	int					replace_flag;
	int					delimiter_token;
	struct s_heredoc	*heredoc;
	struct s_struct		*next;
	struct s_struct		*prev;
}	t_struct;

typedef struct s_chain
{
	char				*content;
	int					is_command;
	int					is_pipe;
	int					is_redirect_in;
	int					is_redirect_out;
	int					is_append;
	int					is_heredoc;
	int					is_var;
	int					is_empty_var;
	int					hdc_quotes;
	int					last_heredoc;
	int					command_index;
	struct s_heredoc	*heredoc;
	struct s_chain		*next;
}	t_chain;

typedef struct s_env
{
	char			*env;
	struct s_env	*next;
}	t_env;

typedef struct s_var
{
	int				flag;
	int				pos;
	t_env			*env;
}	t_var;

typedef struct s_data
{
	int		args;
	int		pipes;
	int		count;
	int		malloc_count;
	int		heredoc_flag;
	int		heredoc_seen;
	int		heredoc_pos;
	int		read_right;
	int		delimiter_flag;
	int		error;
	int		pipe_flag;
	int		signal;
	int		status;
	int		pos;
	int		start;
	int		run;
	int		empty_var;
	int		builtin_error;
	int		open_quote;
	int		i;
	int		j;
	char	c;
	char	*oldpwd;
	int		add_history;
	char	*full_cmd;
	char	*cmd;
	char	*home;
	char	*dir;
	t_env	*env_head;
	char	**my_env;
	char	*last_arg;
}	t_data;

typedef struct s_redirect
{
	char	*in_file;
	char	*out_file;
	int		is_append;
	int		error;
}	t_redirect;

/*
	The g_signal global variable is used in the what_to_do() function 
	to check if a heredoc was exited prematurely, in which case the command 
	should not be executed.
*/

extern int	g_signal;

char	*get_path(char **env, char **cmd);
void	exec_init(t_chain *chain_head, t_chain **itr, t_struct **head,
			t_data *data);
void	init_pipex(char *cmd0, char *cmd1);
void	pipex_init(t_chain **itr, t_struct **head, t_data *data, char *cmd);
void	solo_exec(t_chain **itr, t_struct **head, t_data *data, char *cmd);
void	solo_exec_init(t_chain **itr, t_struct **head, t_data *data, char *cmd);

// Parsing
void	init_parsing(char *cmd, t_struct *head, t_data *data);
void	count_quotes_heredoc(char *cmd, t_data *data, int *j);
void	args_parsing(char *cmd, t_data *data, t_struct *itr, int j);
void	delimiters_parsing(char *cmd, t_struct *itr, t_data *data);
void	unclosed_pipe_parsing(t_struct *itr, char *cmd, t_data *data);
void	check_parsing_error_pipe(t_struct *itr, int *error);
void	check_parsing_error_redirect_in(t_struct *itr, int *error);
void	check_parsing_error_redirect_out(t_struct *itr, int *error);
void	check_parsing_error_append(t_struct *itr, int *error);
void	check_parsing_error_heredoc(t_struct *itr, int *error);
int		check_parsing_error(t_struct **itr, t_data *data, int error);

// Parsing Var
void	var_env_count_init(char *cmd, int *j, t_data *data);
void	var_count_status(int *j, t_data *data);

// Parsing Utils
void	malloc_next(t_struct **itr);
void	init_parsing_itr(t_struct *itr, t_data *data);
char	get_first_quote(char *cmd);
int		is_delimiter_check(char c);
void	go_back_parsing(t_struct **itr, t_data *data);
void	empty_quote(t_struct *itr, t_data *data);
int		valid_quotes_parsing(t_data *data, int *open);
int		heredoc_flag(t_data *data);

// Executor

void	what_to_do(t_struct **head, t_struct *itr, t_data *data, char *cmd);
t_chain	*make_list_command_init(t_struct **itr, t_data *data, int i,
			int var_flag);
void	make_list_command(t_chain **chain, t_struct **itr,
			t_data *data, int *i);
void	make_list_content_init(t_chain **chain, t_struct *itr, t_data *data);
int		check_dir(char **cmd, t_chain *itr, t_struct **head, t_data *data);
void	check_full_slash(char **cmd, t_chain *itr, t_struct **head,
			t_data *data);
char	*get_relative_path_init(char **env, char *cmd, int i);
void	try_absolute_path(char **tab, char **env);

// Executor Utils
int		skip_new_line(t_struct **itr, t_data *data);
char	**ft_tab_cpy(char **tab);
void	free_tab(char **tab);
void	list_command_init(t_chain *chain);
t_chain	*make_list_malloc_first(t_chain **head);
void	make_list_malloc(t_chain **chain);
void	export_underscore(char *var, t_data *data);
int		check_empty_var(t_struct **itr);
void	fill_cmd(t_chain **itr, char **cmd, int *j);

//	Environment variables
void	make_list_command_var(t_struct **itr, t_chain *chain,
			char **var, int *var_flag);
void	get_last_arg(t_data *data, char *var, int var_flag, int end_flag);
void	export_arg_var(t_data *data, t_chain *chain_head);
void	replace_var(char *content, int *j, t_data *data, t_chain **chain);
void	replace_var_env_init(char *itr_cont, int *j,
			t_data *data, t_chain **chain);

// Pipex
void	malloc_heredoc_next(t_struct **itr);
void	fill_heredoc_node(t_data *data, t_struct **itr, char *heredoc);
void	var_replace_heredoc(char *heredoc, int *j, t_data *data,
			t_struct **itr);
void	make_heredoc_var_count(t_data *data, char *heredoc, int *j);
void	make_var_count(t_data *data, char *heredoc, int *j, int open);
void	var_env_search_heredoc(t_data *data, char *str);
void	put_heredoc_var(t_data *data, char *cmd, int pipe_hdc[2]);
void	var_env_replace_heredoc(char *str, int *j, t_data *data,
			t_struct **itr);
void	make_heredoc_init(t_struct **itr, t_data *data);
int		check_final_redirect(t_struct **itr, t_data *data);
pid_t	*malloc_pid(t_data *data, int **pid);
int		**malloc_pipes(t_data *data);
void	init_pipes(int **pipes, t_data *data);
void	free_pipes(int **pipes);
void	init_waitpid(int *pid, int **pipes, t_data *data);
t_chain	*check_redirect_init(t_chain *itr, t_redirect *redirect, int i);
void	make_exec_redirect(int *fd, t_redirect redirect);
void	close_all(int **pipes, t_data *data);
int		first_pipe(t_chain *itr, int **pipes, t_data *data, char *cmd);
int		inter_pipe(t_chain *itr, int **pipes, t_data *data, char *cmd);
int		last_pipe(t_chain *itr, int **pipes, t_data *data, char *cmd);
void	pipex_init(t_chain **itr, t_struct **head, t_data *data, char *cmd);

// Environment init
void	malloc_env_array(t_data *data, t_env *head);
t_env	*malloc_env(char **env);
void	print_env(char **cmd, t_data *data, t_chain *itr, t_struct **head);
void	malloc_home(t_data *data);
void	update_env(t_data *data, t_env **new_node, char *var);

// Signals
void	signals(void);
void	signals_in_exec(void);
void	flush_prompt(int sig);
void	in_command_prompt(int sig);
void	in_heredoc_prompt(int sig);
void	miniception_signals(int sig);

// General builtins
int		redefine_var(t_env *env_list, char *cmd, int i);
int		is_builtin(char *command);
void	exec_builtin(char **cmd, t_data *data, t_chain *itr, t_struct **head);
void	echo(char **cmd, t_data *data, t_chain *itr, t_struct **head);
void	export(char **cmd, t_data *data, int i, int j);
void	print_sort_env(t_data *data);
t_env	*add_env_node(char *var);
void	copy_var(char *cmd, t_env *env_list);
void	redefine_home(t_data *data, char *cmd);
void	unset(char **cmd, t_data *data, t_env *head, int i);
char	**realloc_env_array(char ***new_env, t_data *data);
void	exit_builtin(char **cmd, t_data *data, t_chain *itr, t_struct **head);
int		ft_exit_isdigit(char *str);
void	export_error(t_data *data, char *var);
int		ft_strcmp(const char *s1, const char *s2);
void	get_dir(t_data *data);

// Cd builtin
void	cd(char **cmd, t_data *data, char **env);
int		check_cd_args(char **cmd, t_data *data);
char	*get_path_cd(char **cmd, char **env, t_data *data);
void	do_chdir(char *dir, t_data *data);
void	check_unset_pwd(t_env *env_list);
void	error_no_access(char *dir, t_data *data);
char	*invalid_path(t_data *data, t_env *env, char *path);

// Exit
void	ft_exit(char **cmd, t_data *data, t_chain *itr, t_struct **head);
void	ft_exit_solo(t_data *data, t_chain *itr, t_struct **head);

// Error Handling
int		error_check(char *cmd, t_data *data, int i, int flag);
void	error(char *msg);
int		check_pipe_error(t_struct *itr);
int		check_unclosed_pipe(t_struct *itr);
void	unclosed_pipe(t_struct *itr, t_data *data, t_struct **head);
void	print_error_message(t_data *data);

// Utils
void	div_and_get_nb(t_data *data, int *pos, int *nb, int *j);
void	free_list(t_struct *head);
void	free_chain(t_chain *head);
void	free_array(char **array);
void	free_env(t_env *head);
void	init_data(t_data *data);
void	reinit_data(t_data *data);
void	print_error_stderr(char *cmd, char *str);
void	print_error_stderr_2(char *cmd, char *str, char *str_2);
void	init_shlvl(t_data *data);

// History
void	make_add_history(char *cmd, t_data *data, int i, int flag);
void	newline_history(char *cmd, t_struct **itr, t_data *data);

// Heredoc
void	heredoc_redir(int pipe_hdc[2], t_chain *hdc, t_data *data, char *cmd);
int		check_heredoc_init(t_struct **itr, t_data *data);
void	make_heredoc_init(t_struct **itr, t_data *data);
void	eof_in_heredoc(t_struct **itr);

#endif