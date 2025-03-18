/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:15:52 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/27 18:04:17 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_parsing_error_pipe(t_struct *itr, int *error)
{
	if (itr->prev == NULL)
	{
		print_error_stderr(NULL, "syntax error near unexpected token `|'");
		*error = 1;
	}
	else if (itr->prev && (itr->prev->delimiter_token == 1
			|| itr->prev->is_pipe == 1
			|| (itr->prev->is_heredoc && itr->prev->content == NULL)))
	{
		print_error_stderr(NULL, "syntax error near unexpected token `|'");
		*error = 1;
	}
}
