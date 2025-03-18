/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:09:17 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/20 13:04:49 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Initializes some variables in the data struct at the start of the program.

void	init_data(t_data *data)
{
	data->heredoc_pos = -1;
	data->status = 0;
	data->last_arg = NULL;
	data->full_cmd = NULL;
	data->dir = NULL;
}
/*
	Initializes all the other data variables that need to be re-initialized
	for every command. Runs at the start of every loop in the main().
*/

void	reinit_data(t_data *data)
{
	data->heredoc_seen = 0;
	data->args = 0;
	data->pipes = 0;
	data->count = 0;
	data->heredoc_flag = 0;
	data->malloc_count = 0;
	data->pos = 0;
	data->delimiter_flag = 0;
	data->pipe_flag = 0;
	data->run = 1;
	data->empty_var = 0;
	data->i = 0;
	data->j = 0;
	data->c = -1;
	data->open_quote = 0;
	data->error = 0;
	data->builtin_error = 0;
	data->oldpwd = NULL;
	data->add_history = 1;
	data->read_right = 1;
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
	Redirects STDOUT to STDERR in order to write an error message
	with printf in the standard error. The STDOUT is then restored.
*/

void	print_error_stderr(char *cmd, char *str)
{
	int	fd;
	int	fd_out;

	fd = dup(STDERR_FILENO);
	fd_out = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	if (cmd != NULL)
		printf("%s :", cmd);
	printf("%s\n", str);
	dup2(STDOUT_FILENO, fd_out);
}

void	print_error_stderr_2(char *str, char *cmd, char *str_2)
{
	int	fd;
	int	fd_out;

	fd = dup(STDERR_FILENO);
	fd_out = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	printf("%s%s%s\n", str, cmd, str_2);
	dup2(STDOUT_FILENO, fd_out);
	return ;
}
