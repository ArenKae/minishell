/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 07:15:41 by keschouf          #+#    #+#             */
/*   Updated: 2024/03/28 09:04:42 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_heredoc_1(t_struct **itr, t_data *data, char *heredoc, int *j)
{
	data->i = 0;
	while (heredoc[data->i])
	{
		if (heredoc[data->i] == '$' && (*itr)->hdc_quotes == 0)
			make_heredoc_var_count(data, heredoc, j);
		data->i++;
		*j += 1;
	}
}

void	make_heredoc_2(t_struct **itr, int count, int j)
{
	if (count)
		malloc_heredoc_next(itr);
	(*itr)->heredoc->line = malloc(sizeof(char) * (j + 2));
}

void	heredoc_null_line(t_struct **itr, int count)
{
	if (!count)
		(*itr)->heredoc->line = NULL;
}

/*
	If a valid heredoc was found during parsing, it is created here.
	First, the standard input fd is saved with a dup in case the heredoc is
	interrupted by a Ctrl + D signal (which triggers the closing of STDIN to 
	bypass the heredoc). The global_signal_status() will update the status of
	the global variable in case the heredoc is exited via a Ctrl + D.
	If the heredoc delimiter is found by ft_strncmp(), the heredoc list is null
	terminated and user input stops.
*/

void	make_heredoc(t_struct **itr, t_data *data, int count, int j)
{
	int			fd;
	char		*heredoc;

	fd = dup(STDIN_FILENO);
	while (1)
	{
		heredoc = readline("heredoc>");
		dup2(fd, STDIN_FILENO);
		if (heredoc == NULL)
		{
			eof_in_heredoc(itr);
			break ;
		}
		if (!ft_strncmp(heredoc, (*itr)->content,
				(ft_strlen((*itr)->content) + 1)))
		{
			heredoc_null_line(itr, count);
			break ;
		}
		make_heredoc_1(itr, data, heredoc, &j);
		make_heredoc_2(itr, count, j);
		fill_heredoc_node(data, itr, heredoc);
		count++;
		free(heredoc);
	}
	free(heredoc);
}

/*
	Initializes the creation of a heredoc.
	The signal() function handles the SIGINT (Ctrl + c) input.
	A head to the heredoc struct is allocated to store the 
	content of the heredoc.
*/

void	make_heredoc_init(t_struct **itr, t_data *data)
{
	t_heredoc	*tmp;

	data->i = 0;
	data->run = 0;
	signal(SIGINT, in_heredoc_prompt);
	(*itr)->heredoc = malloc(sizeof(t_heredoc));
	if (!(*itr)->heredoc)
		return (perror("malloc"));
	(*itr)->is_heredoc = 1;
	tmp = (*itr)->heredoc;
	(*itr)->heredoc->line = NULL;
	make_heredoc(itr, data, 0, 0);
	(*itr)->heredoc->next = NULL;
	(*itr)->heredoc = tmp;
}
