/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:49:49 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 09:52:59 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	*malloc_pid(t_data *data, int **pid)
{
	*pid = malloc(sizeof(pid_t) * (data->pipes + 1));
	if (!(*pid))
		return (NULL);
	return (*pid);
}

int	**malloc_pipes(t_data *data)
{
	int	i;
	int	**tab;

	i = 0;
	tab = malloc(sizeof(int *) * (data->pipes + 1));
	if (!tab)
		return (NULL);
	while (i < data->pipes)
	{
		tab[i] = malloc(sizeof(int) * 2);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

void	init_pipes(int **pipes, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pipes)
	{
		if (pipe(pipes[i]) < 0)
			perror("pipe");
		i++;
	}
}

void	free_pipes(int **pipes)
{
	int	i;

	i = 0;
	while (pipes[i])
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	init_waitpid(int *pid, int **pipes, t_data *data)
{
	int	status;

	close_all(pipes, data);
	data->i = 0;
	while (data->i <= data->pipes)
	{
		waitpid(pid[data->i], &status, 0);
		data->i++;
	}
	if (WIFEXITED(status))
		data->status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		status += 128;
		data->status = status;
	}
	free(pid);
	free_pipes(pipes);
}
