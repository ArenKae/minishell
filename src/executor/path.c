/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:11:10 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 06:37:30 by acosi            ###   ########.fr       */
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
