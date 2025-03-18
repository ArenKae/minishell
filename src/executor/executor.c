/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:22:56 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 06:49:01 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Counts the number of arguments in the command. This number will 
	be used to malloc the correct number of elements in the array 
	that is sent to execve().
*/

int	args_count(t_chain *itr)
{
	int	nb;

	nb = 1;
	while (itr->next)
	{
		if (itr->content == NULL && itr->next)
		{
			itr = itr->next;
			continue ;
		}
		else
			itr = itr->next;
		if (itr->is_pipe == 1)
			break ;
		if (itr->is_redirect_in != 1 && itr->is_redirect_out != 1
			&& itr->is_append != 1 && itr->is_heredoc != 1)
			nb++;
	}
	return (nb);
}

/*
	Executes the command stored inside the array cmd along with its
	arguments and options.
	The function will first try to check if an absolute or relative path 
	was sent and check for errors while opening the associated file or 
	directory. If path stays at NULL, it means we recieaved an empty 
	command and the process will exit with the right status.
	Finally, we try to acess the binary or executable with execve. If 
	successful, execve will detach from the calling process and not 
	print the following error messages.
*/

int	exec(char **cmd, t_chain *itr, t_struct **head, t_data *data)
{
	char	*path;

	path = NULL;
	if (cmd[0] == NULL)
		ft_exit(cmd, data, itr, head);
	check_full_slash(cmd, itr, head, data);
	try_absolute_path(cmd, data->my_env);
	if (!check_dir(cmd, itr, head, data))
		path = get_relative_path_init(data->my_env, cmd[0], 0);
	if (path == NULL)
	{
		data->status = 127;
		ft_exit(cmd, data, itr, head);
	}
	execve(path, cmd, data->my_env);
	print_error_stderr(cmd[0], " command not found");
	data->status = 127;
	free(path);
	ft_exit(cmd, data, itr, head);
	return (0);
}

/*
	Malloc an array holding the command to execute and send it to the right 
	function for execution. The struct itr is used to count the number of 
	agruments to malloc an array that will be used by execve(). While no 
	redirect flags are found, the array is filled. If a pipe is found, the 
	array is null-terminated so that only what is on the same side of the pipe 
	will be executed. Finally, if the command holds a builtin, it will be 
	executed as such by the sentinel function exec_builtin(). Otherwise, 
	it is sent to exec() for an execve call.
*/

void	exec_init(t_chain *chain_head, t_chain **itr,
	t_struct **head, t_data *data)
{
	char	**cmd;
	int		j;

	j = 0;
	cmd = malloc(sizeof(char *) * (args_count(*itr) + 1));
	if (!cmd)
		return ;
	while (*itr)
	{
		if ((*itr)->content == NULL && (*itr)->next)
		{
			(*itr) = (*itr)->next;
			if (*itr && ((*itr)->is_pipe == 1))
				break ;
			continue ;
		}
		fill_cmd(itr, cmd, &j);
		if (*itr && ((*itr)->is_pipe == 1))
			break ;
	}
	cmd[j] = NULL;
	if (is_builtin(cmd[0]))
		exec_builtin(cmd, data, chain_head, head);
	else
		exec(cmd, chain_head, head, data);
}
