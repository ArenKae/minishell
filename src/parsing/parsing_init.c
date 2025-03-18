/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:28:27 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 21:03:31 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Skip to the next argument in case a '#' character is encountered.

void	get_next_arg(char *cmd, t_data *data)
{
	while (cmd[data->pos] && cmd[data->pos] != '\n')
		data->pos++;
}

/*
    Malloc a new node for the itr struct.
	First, the command is parsed by count_quotes_parsing() to account for 
	the quotes and calculates the appropriate size to malloc the node.
	If an empty (= non-existent) var was found during this parsing, nothing
	is allocated.
	Else, the correct size is malloc'd for this node and the cmd is parsed 
	a second time to copy its content in the nodes of the list.
*/

void	init_args_parsing(char *cmd, t_struct *itr, t_data *data)
{
	int			j;

	j = 0;
	data->i = data->pos;
	count_quotes_heredoc(cmd, data, &j);
	if (data->empty_var == 1 && j == 0)
	{
		data->pos = data->i;
		itr->content = NULL;
		itr->is_empty_var = 1;
		data->empty_var = 0;
		return ;
	}
	if (j > 0)
	{
		itr->content = malloc(sizeof(char) * (j + 1));
		if (!itr->content)
			return (perror("malloc"));
		args_parsing(cmd, data, itr, -1);
		data->delimiter_flag = 0;
		data->heredoc_flag = 0;
	}
	if (j == 0)
		empty_quote(itr, data);
}

/*
    Parses the cmd line and malloc a node to store each argument in the linked
	list. The first if() ensures that the list is null-terminated. If a '#' 
	is found, we go backwards and exit the function to enter get_next_arg() 
	from the higher function init_parsing. If data->count, it means we are not 
	on the first node and we malloc the next one. The heredoc_flag is used to 
	store any delimiter chain in the same node as the one with the flag. When 
	a newline is found, the cmd is added to the history. Finally, the parsing 
	is split between normal characters and delimiters.
*/

void	parsing_and_malloc(char *cmd, t_struct **itr, t_data *data)
{
	while ((*itr) && ((*itr)->is_pipe == 1
			&& (cmd[data->pos] == '\n' || cmd[data->pos] == ' ')))
		data->pos++;
	if (!cmd[data->pos])
	{
		(*itr)->next = NULL;
		return ;
	}
	if (cmd[data->pos] == '#')
		return (go_back_parsing(itr, data));
	if ((cmd[data->pos] == '|' || cmd[data->pos] == '<'
			|| cmd[data->pos] == '>' || cmd[data->pos] == '\n'))
		data->heredoc_flag = 0;
	if (data->count && data->heredoc_flag != 1)
		malloc_next(itr);
	if (cmd[data->pos] == '\n')
		newline_history(cmd, itr, data);
	else if (cmd[data->pos] != '|' && cmd[data->pos] != '<'
		&& cmd[data->pos] != '>')
		init_args_parsing(cmd, *itr, data);
	else if (cmd[data->pos])
		delimiters_parsing(cmd, *itr, data);
	data->count++;
	(*itr)->next = NULL;
}

/*
	Parses the command line 'cmd' and malloc a new node
	for the linked list 'itr' every time a space is encountered.
	data->pos is an index initialized at 0 and used to parse cmd.
	It is stored in the data struct to keep easy track of the 
	moving position on the cmd string across multiple functions.
	If a '#' character is encountered, what follows is a comment and
	shouldn't be interpreted, so we skip to the next argument.
	Else, if a space or a tab is encoutered, then what follows is
	parsed and malloc'd as a new node of the struct itr.
	
*/

void	init_parsing(char *cmd, t_struct *head, t_data *data)
{
	t_struct	*itr;

	data->count = 0;
	itr = head;
	init_parsing_itr(itr, data);
	while (cmd[data->pos])
	{
		if (cmd[data->pos] == '#')
		{
			get_next_arg(cmd, data);
			itr->next = NULL;
		}
		else
		{
			if (cmd[data->pos] != ' ' && cmd[data->pos] != '\t')
				parsing_and_malloc(cmd, &itr, data);
			data->pos++;
		}
	}
	if (data->heredoc_flag == 1)
		itr->content = NULL;
	if (data->add_history)
		make_add_history(cmd, data, 0, 0);
}

// Parses and malloc the new line prompted after an unclosed pipe.

void	unclosed_pipe_parsing(t_struct *itr, char *cmd, t_data *data)
{
	data->pos = 0;
	while (itr->next != NULL)
		itr = itr->next;
	while (cmd[data->pos])
	{
		if (cmd[data->pos] == '#')
		{
			get_next_arg(cmd, data);
			itr->next = NULL;
		}
		else
		{
			if (cmd[data->pos] != ' ' && cmd[data->pos] != '\t')
				parsing_and_malloc(cmd, &itr, data);
			data->pos++;
		}
	}
}
