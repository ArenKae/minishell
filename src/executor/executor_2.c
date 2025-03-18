/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:22:56 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/02 03:30:54 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	If a valid absolute path was inputed and the calling process has access 
	to it, the name of the binary is extracted and used by execve().
*/

void	try_absolute_path(char **tab, char **env)
{
	char	*path;
	char	**cmd;

	cmd = ft_tab_cpy(tab);
	path = ft_strdup(cmd[0]);
	if (access(cmd[0], F_OK) == 0)
		make_absolute_path(cmd);
	execve(path, cmd, env);
	free_tab(cmd);
	free(path);
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

void	make_list_command_var(t_struct **itr, t_chain *chain,
	char **var, int *var_flag)
{
	if (chain->content != NULL && !chain->is_empty_var)
		*var = chain->content;
	if (!ft_strncmp((*itr)->content, "$_\0", 3))
		*var_flag = 1;
	else
		*var_flag = 0;
	(*itr) = (*itr)->next;
	chain->next = NULL;
}

void	get_last_arg( t_data *data, char *var, int var_flag, int end_flag)
{
	if (var_flag == 1)
		return ;
	if (!var)
	{
		if (data->last_arg != NULL)
			free(data->last_arg);
		data->last_arg = NULL;
		return ;
	}
	if (data->last_arg != NULL)
		free(data->last_arg);
	data->last_arg = ft_strdup(var);
	if (end_flag == 1)
		data->run = 0;
}
