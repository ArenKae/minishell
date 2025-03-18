/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:15:01 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/29 14:13:56 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_parsing_error_redirect_in(t_struct *itr, int *error)
{
	if (itr->prev && (itr->prev->delimiter_token == 1
			|| (itr->prev->is_heredoc && itr->prev->content == NULL)))
	{
		print_error_stderr(NULL, "syntax error near unexpected token `<'");
		*error = 1;
	}
}

void	check_parsing_error_redirect_out(t_struct *itr, int *error)
{
	if (itr->prev && (itr->prev->delimiter_token == 1
			|| (itr->prev->is_heredoc && itr->prev->content == NULL)))
	{
		print_error_stderr(NULL, "syntax error near unexpected token `>'");
		*error = 1;
	}
}

void	check_parsing_error_append(t_struct *itr, int *error)
{
	if (itr->prev && (itr->prev->delimiter_token == 1
			|| (itr->prev->is_heredoc && itr->prev->content == NULL)))
	{
		print_error_stderr(NULL, "syntax error near unexpected token `>>'");
		*error = 1;
	}
}

void	check_parsing_error_heredoc(t_struct *itr, int *error)
{
	if (itr->prev && (itr->prev->delimiter_token == 1
			|| (itr->prev->is_heredoc && itr->prev->content == NULL)))
	{
		print_error_stderr(NULL, "syntax error near unexpected token `<<'");
		*error = 1;
	}
}

int	check_parsing_error1(t_struct **itr, t_data *data, int error)
{
	t_struct	*tmp;

	tmp = *itr;
	while ((*itr) && (*itr)->is_new_line != 1)
	{
		if ((*itr)->is_pipe == 1 && error == 0)
			check_parsing_error_pipe(*itr, &error);
		else if ((*itr)->is_redirect_in == 1 && error == 0)
			check_parsing_error_redirect_in(*itr, &error);
		else if ((*itr)->is_redirect_out == 1 && error == 0)
			check_parsing_error_redirect_out(*itr, &error);
		else if ((*itr)->is_append == 1 && error == 0)
			check_parsing_error_append(*itr, &error);
		else if ((*itr)->is_heredoc == 1 && error == 0)
			check_parsing_error_heredoc(*itr, &error);
		(*itr) = (*itr)->next;
	}
	if (!error)
		(*itr) = tmp;
	else if ((*itr) && (*itr)->is_new_line == 1)
		(*itr) = (*itr)->next;
	if (error)
		data->status = 2;
	return (error);
}
