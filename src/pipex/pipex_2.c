/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:31:56 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/29 14:10:14 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Handles the first pipe. First we take care of any redirections if there 
	are any. If not, STDOUT is redirected to the WRITE end of the pipe. 
	Finally, if a heredoc is present, it is redirected here as well.
*/

int	first_pipe(t_chain *itr, int **pipes, t_data *data, char *cmd)
{
	t_redirect	redirect;
	t_chain		*hdc;
	int			fd;
	int			pipe_hdc[2];

	hdc = check_redirect_init(itr, &redirect, data->i);
	make_exec_redirect(&fd, redirect);
	if (!redirect.out_file)
		dup2(pipes[data->i][WRITE], STDOUT);
	if (hdc)
		heredoc_redir(pipe_hdc, hdc, data, cmd);
	return (redirect.error);
}

// Get the READ end of the previous pipe and redirect it
// to the WRITE end of the inter pipe.

int	inter_pipe(t_chain *itr, int **pipes, t_data *data, char *cmd)
{
	t_redirect	redirect;
	t_chain		*hdc;
	int			fd;
	int			pipe_hdc[2];

	hdc = check_redirect_init(itr, &redirect, data->i);
	dup2(pipes[data->i - 1][READ], STDIN);
	dup2(pipes[data->i][WRITE], STDOUT);
	make_exec_redirect(&fd, redirect);
	if (hdc)
		heredoc_redir(pipe_hdc, hdc, data, cmd);
	return (redirect.error);
}

int	last_pipe(t_chain *itr, int **pipes, t_data *data, char *cmd)
{
	t_redirect	redirect;
	t_chain		*hdc;
	int			fd;
	int			pipe_hdc[2];

	hdc = check_redirect_init(itr, &redirect, data->i);
	dup2(pipes[data->i - 1][0], 0);
	make_exec_redirect(&fd, redirect);
	if (hdc)
		heredoc_redir(pipe_hdc, hdc, data, cmd);
	return (redirect.error);
}
