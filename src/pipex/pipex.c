/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:03:06 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 09:21:00 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Util to close all pipes in a single function.

void	close_all(int **pipes, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

/*
	Creates a chain of pipes to ensure communication between all the processes 
	that were forked. The first and last pipe are handled separately.
*/

int	do_pipex(t_chain *itr, t_data *data, char *cmd, int **pipes)
{
	int	error;

	error = 0;
	if (data->i == 0)
		error = first_pipe(itr, pipes, data, cmd);
	else if (data->i > 0 && data->i < data->pipes)
		error = inter_pipe(itr, pipes, data, cmd);
	else if (data->i == data->pipes)
		error = last_pipe(itr, pipes, data, cmd);
	close_all(pipes, data);
	return (error);
}

void	pipex_init_2(t_chain **itr, t_data *data)
{
	while ((*itr)->next)
	{
		*itr = (*itr)->next;
		if ((*itr)->is_pipe == 1)
			break ;
	}
	data->i++;
}

/*
	Malloc pipes and pid depending on the number of pipes that were found 
	during parsing. The newly created pipes are then created with the 
	pipe() call in init_pipes.
*/

void	pipex_init_pipes_and_pid(t_data *data, int ***pipes, pid_t **pid)
{
	*pid = malloc_pid(data, pid);
	*pipes = malloc_pipes(data);
	init_pipes(*pipes, data);
}

/*
	Create pipes and pids for each block of command between a pipe, before 
	forking each into a new child process. do_pipex will create the chain 
	of pipes while pipex_init_2() will go to the next pipe. Finally, the 
	waitpid() call is used to synchronize all the processes.
*/

void	pipex_init(t_chain **itr, t_struct **head, t_data *data, char *cmd)
{
	pid_t	*pid;
	int		**pipes;
	t_chain	*chain_head;
	int		error;

	data->i = 0;
	chain_head = *itr;
	pipex_init_pipes_and_pid(data, &pipes, &pid);
	signals_in_exec();
	while (data->i <= data->pipes)
	{
		pid[data->i] = fork();
		if (pid[data->i] == 0)
		{
			error = do_pipex(*itr, data, cmd, pipes);
			free(pid);
			free_pipes(pipes);
			if (error)
				ft_exit_solo(data, chain_head, head);
			exec_init(chain_head, itr, head, data);
		}
		pipex_init_2(itr, data);
	}
	close_all(pipes, data);
	init_waitpid(pid, pipes, data);
}
