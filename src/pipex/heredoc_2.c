/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:55:33 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/18 23:46:11 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	If the heredoc is followed by a newline and some additionnal commands, they 
	should not be interpreted, but only printed before the result of the heredoc.
	This function parses the heredoc line and stores what's after the newline 
	to print it later. It also checks if there is an environment variable to 
	access or display. Then, it writes the content of the heredoc in the WRITE 
	end of the pipe.
*/

void	put_heredoc(int pipe_hdc[2], t_chain *hdc, t_data *data, char *cmd)
{
	if (data->heredoc_pos != -1)
	{
		while (cmd[data->heredoc_pos] != '\n' && cmd[data->heredoc_pos] != '\0')
			data->heredoc_pos++;
		if (cmd[data->heredoc_pos] == '\n')
		{
			data->heredoc_pos++;
			while (cmd[data->heredoc_pos] != '\0')
			{
				if (hdc->hdc_quotes != 1 && cmd[data->heredoc_pos] == '$')
					put_heredoc_var(data, cmd, pipe_hdc);
				ft_putchar_fd(cmd[data->heredoc_pos], pipe_hdc[WRITE]);
				data->heredoc_pos++;
			}
			ft_putchar_fd('\n', pipe_hdc[WRITE]);
		}
	}
	while (hdc->heredoc)
	{
		ft_putstr_fd(hdc->heredoc->line, pipe_hdc[1]);
		hdc->heredoc = hdc->heredoc->next;
	}
}

//	Creates a pipe for the heredoc process and handles writing inside it.

void	heredoc_redir(int pipe_hdc[2], t_chain *hdc, t_data *data, char *cmd)
{
	pid_t	pid;

	if (pipe(pipe_hdc) < 0)
		return (perror("pipe"));
	pid = fork();
	if (pid == CHILD)
	{
		close(pipe_hdc[READ]);
		put_heredoc(pipe_hdc, hdc, data, cmd);
		exit(EXIT_SUCCESS);
	}
	dup2(pipe_hdc[READ], STDIN);
	close(pipe_hdc[WRITE]);
}

void	malloc_heredoc_next(t_struct **itr)
{
	(*itr)->heredoc->next = malloc(sizeof(t_heredoc));
	(*itr)->heredoc = (*itr)->heredoc->next;
}

void	fill_heredoc_node(t_data *data, t_struct **itr, char *heredoc)
{
	int	j;

	j = 0;
	data->pos = 0;
	while (heredoc[data->pos])
	{
		if (heredoc[data->pos] == '$' && (*itr)->hdc_quotes == 0)
			var_replace_heredoc(heredoc, &j, data, itr);
		else
		{
			(*itr)->heredoc->line[j] = heredoc[data->pos];
			j++;
		}
		data->pos++;
	}
	(*itr)->heredoc->line[j] = '\n';
	(*itr)->heredoc->line[j + 1] = '\0';
}

/*
	Update the status of the global variable in case the heredoc is
	exited via a Ctrl + D, so that the correct exit code is returned
	in init_minishell().
*/

void	eof_in_heredoc(t_struct **itr)
{
	char	*str;

	str = "warning: here-document delimited by end-of-file (wanted `";
	if (g_signal != CTRL_C_HEREDOC)
	{
		print_error_stderr_2(str, (*itr)->content, "')");
		g_signal = CTRL_D;
	}
}
