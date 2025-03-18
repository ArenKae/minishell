/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:29:27 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/29 14:11:02 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Initializes data inside the heredoc and redirect struct.

void	heredoc_and_redirect_init(t_chain **heredoc, t_redirect *redirect)
{
	*heredoc = NULL;
	redirect->in_file = NULL;
	redirect->out_file = NULL;
	redirect->is_append = 0;
	redirect->error = 0;
}

//	Prints an error message if the redirection file could not be opened.

void	print_rights_error(char *str, t_redirect *redirect)
{
	redirect->error = 1;
	perror(str);
}

/*
	Parses through the itr list and check for redirection flags in the current 
	node. If a input redirection (<) is found, the path to the input_file is 
	stored in the redirect structure, and we try to open the file. If the file 
	can't be opened, an error message will be displayed. Similarly, we do the 
	same for output and append redirection, with the appropriate open options.
*/

void	check_redirect(t_chain *itr, t_redirect *redirect, int fd)
{
	if (itr->is_redirect_in == 1)
	{
		redirect->in_file = itr->content;
		fd = open(redirect->in_file, O_RDONLY, S_IRWXU);
		if (fd == -1)
			print_rights_error(itr->content, redirect);
	}
	else if (itr->is_redirect_out == 1)
	{
		redirect->out_file = itr->content;
		fd = open(redirect->out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			print_rights_error(itr->content, redirect);
	}
	else if (itr->is_append == 1)
	{
		redirect->out_file = itr->content;
		redirect->is_append = 1;
		fd = open(redirect->out_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if (fd == -1)
			print_rights_error(itr->content, redirect);
	}
}

/*
	Creates and fill the heredoc and redirect struct for later use during 
	redirections. Also check for redirections errors.
*/

t_chain	*check_redirect_init(t_chain *itr, t_redirect *redirect, int i)
{
	int		fd;
	int		heredoc_flag;
	t_chain	*heredoc;

	fd = 0;
	heredoc_flag = 0;
	heredoc_and_redirect_init(&heredoc, redirect);
	while (itr && itr->command_index == i && redirect->error == 0)
	{
		check_redirect(itr, redirect, fd);
		if (itr->is_heredoc == 1)
		{
			heredoc = itr;
			heredoc_flag = 1;
		}
		else if (itr->is_redirect_in == 1)
			heredoc_flag = 0;
		itr = itr->next;
	}
	if (heredoc_flag == 1)
		return (heredoc);
	else
		return (NULL);
}

//	Redirects the fd for the input/ouput file when inside a pipe.

void	make_exec_redirect(int *fd, t_redirect redirect)
{
	if (redirect.in_file)
	{
		*fd = open(redirect.in_file, O_RDONLY, S_IRWXU);
		dup2(*fd, STDIN);
	}
	if (redirect.out_file && redirect.is_append != 1)
	{
		*fd = open(redirect.out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		dup2(*fd, STDOUT);
	}
	else if (redirect.out_file)
	{
		*fd = open(redirect.out_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
		dup2(*fd, STDOUT);
	}
}
