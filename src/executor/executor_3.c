/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:11:10 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/30 19:00:51 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_relative_path(char **env, char *cmd, int i)
{
	char	**paths;
	char	*cmd_to_exec;
	char	*path_to_exec;

	paths = ft_split(env[i] + 5, ':');
	cmd_to_exec = ft_strjoin("/", cmd);
	i = 0;
	while (paths[i] != NULL)
	{
		path_to_exec = ft_strjoin(paths[i], cmd_to_exec);
		if (access(path_to_exec, F_OK) == 0)
		{
			free_tab(paths);
			free(cmd_to_exec);
			return (path_to_exec);
		}
		free(path_to_exec);
		i++;
	}
	free_tab(paths);
	free(cmd_to_exec);
	return (NULL);
}

char	*get_relative_path_init(char **env, char *cmd, int i)
{
	char	*path_to_exec;

	path_to_exec = NULL;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5) != NULL)
			break ;
		i++;
	}
	if (env[i] != NULL)
		path_to_exec = get_relative_path(env, cmd, i);
	if (path_to_exec != NULL)
		return (path_to_exec);
	print_error_stderr(cmd, " command not found");
	return (NULL);
}

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
	Extracts the name of the binary command to perform from the absolute 
	path that was given. First go to the end of the path, then go backward 
	until a '/' is found. Finally, only name of the binary is copied at the 
	beginning of the cmd array.
*/

void	make_absolute_path(char **cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (access(cmd[0], F_OK) == 0)
	{
		while (cmd[0][i])
			i++;
		while (cmd[0][i] != '/')
			i--;
		i++;
		while (cmd[0][i])
		{
			cmd[0][j] = cmd[0][i];
			i++;
			j++;
		}
		while (cmd[0][j])
		{
			cmd[0][j] = '\0';
			j++;
		}
	}
}

//	After a command was executed (with or without success), its last argument 
//	is retrieved and stored inside the '_' environment variable.

void	export_arg_var(t_data *data, t_chain *chain_head)
{
	char	*full_var;

	if (data->last_arg == NULL)
		return (free_chain(chain_head));
	full_var = ft_strjoin("_=", data->last_arg);
	export_underscore(full_var, data);
	free(full_var);
	free_chain(chain_head);
}
