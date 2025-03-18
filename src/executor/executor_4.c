/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:05:18 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/27 18:06:18 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dir_ok(char *cmd, t_data *data)
{
	print_error_stderr_2("minishell$ ", cmd, ": Is a directory");
	data->status = 126;
}

void	error_dir(char *cmd, t_data *data)
{
	print_error_stderr_2("minishell$ ", cmd, ": No such file or directory");
	data->status = 127;
}

void	error_exec_perm(char *cmd, t_data *data)
{
	print_error_stderr_2("minishell$ ", cmd, ": Permission denied");
	data->status = 126;
}

/*
	Check if the calling process has sufficient permissions to access the 
	file or directory that is called. Prints a matching error otherwise.
*/

int	check_dir(char **cmd, t_chain *itr, t_struct **head, t_data *data)
{
	DIR	*open;
	int	i;

	i = -1;
	while (cmd[0][++i])
	{
		if (cmd[0][i] == '/')
		{
			open = opendir(cmd[0]);
			if (open)
				dir_ok(cmd[0], data);
			else if (access(cmd[0], F_OK) == 0 && access(cmd[0], X_OK) != 0)
				error_exec_perm(cmd[0], data);
			else
				error_dir(cmd[0], data);
			ft_exit(cmd, data, itr, head);
		}
	}
	return (0);
}

//	Checks if the command starts with a '/', meaning it's a directory.

void	check_full_slash(char **cmd, t_chain *itr,
	t_struct **head, t_data *data)
{
	int	i;

	i = 0;
	if (cmd[0][0] == '\0')
		return ;
	while (cmd[0][i])
	{
		if (cmd[0][i] != '/')
			return ;
		i++;
	}
	print_error_stderr_2("minishell$ ", cmd[0], ": Is a directory");
	data->status = 126;
	ft_exit(cmd, data, itr, head);
}
