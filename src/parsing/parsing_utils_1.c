/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:15:08 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/18 09:51:41 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Malloc the next node of the itr struct and initialize its variables.

void	malloc_next(t_struct **itr)
{
	t_struct	*tmp;

	tmp = *itr;
	(*itr)->next = malloc(sizeof(t_struct));
	(*itr) = (*itr)->next;
	(*itr)->prev = tmp;
	(*itr)->content = NULL;
	(*itr)->is_pipe = 0;
	(*itr)->is_redirect_out = 0;
	(*itr)->is_redirect_in = 0;
	(*itr)->is_append = 0;
	(*itr)->is_heredoc = 0;
	(*itr)->is_var = 0;
	(*itr)->is_empty_var = 0;
	(*itr)->expand = 0;
	(*itr)->is_new_line = 0;
	(*itr)->replace_flag = 0;
	(*itr)->hdc_quotes = 0;
	(*itr)->delimiter_token = 0;
	(*itr)->heredoc = NULL;
}

//	Initialize data for the first node of itr struct during parsing.

void	init_parsing_itr(t_struct *itr, t_data *data)
{
	itr->content = NULL;
	itr->is_pipe = 0;
	data->pos = 0;
	data->i = 0;
	data->j = 0;
	data->start = 0;
	itr->is_redirect_out = 0;
	itr->is_redirect_in = 0;
	itr->is_append = 0;
	itr->is_heredoc = 0;
	itr->is_var = 0;
	itr->is_empty_var = 0;
	itr->expand = 0;
	itr->replace_flag = 0;
	itr->heredoc = NULL;
	itr->is_new_line = 0;
	itr->hdc_quotes = 0;
	itr->delimiter_token = 0;
	itr->prev = NULL;
}

//	Returns the position of the first quote character on the string cmd.

char	get_first_quote(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			return (cmd[i]);
		i++;
	}
	return (0);
}

//	Check if the current character is a delimiter during itr struct parsing.

int	is_delimiter_check(char c)
{
	if (c == '\0' || c == ' ' || c == '\t' || c == '|'
		|| c == '<' || c == '>'
		|| c == '\0' || c == '\n' || c == '#')
		return (1);
	return (0);
}

/*
	Decrements the position on the string cmd and null-terminate the 
	current struct node in case a '#' character is found during parsing.
*/

void	go_back_parsing(t_struct **itr, t_data *data)
{
	data->pos--;
	(*itr)->next = NULL;
}
