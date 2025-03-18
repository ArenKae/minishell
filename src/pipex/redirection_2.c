/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 15:05:02 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 01:28:09 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirect_error_message(t_struct **itr, t_data *data)
{
	print_error_stderr(NULL, "syntax error near unexpected token `newline'");
	(*itr) = (*itr)->next;
	data->status = 2;
	return (1);
}

/*
	Checks for errors regarding redirections (< or <<).
	The creation of the heredoc is initialized by make_heredoc_init().
	if the global variable g_signal > 0, it means we recieved an 
	EOF signal inside the heredoc and should stop.
	Else, if something is missing after the redirection delimiter,
	an error message is displayed.	
*/

int	check_final_redirect(t_struct **itr, t_data *data)
{
	t_struct	*start;

	start = *itr;
	while ((*itr) && (*itr)->is_new_line != 1)
	{
		if ((*itr)->is_heredoc == 1 && (*itr)->content != NULL)
			make_heredoc_init(itr, data);
		if (g_signal > 0)
			return (1);
		if ((!(*itr)->next) && ((*itr)->delimiter_token == 1
				|| ((*itr)->is_heredoc && (*itr)->content == NULL)))
			return (redirect_error_message(itr, data));
		(*itr) = (*itr)->next;
	}
	if (((*itr) && (*itr)->is_new_line == 1)
		&& (((*itr)->prev && ((*itr)->prev->delimiter_token))
			|| ((*itr)->prev->is_heredoc && (*itr)->prev->content == NULL)))
		return (redirect_error_message(itr, data));
	(*itr) = start;
	return (0);
}

void	make_heredoc_var_count(t_data *data, char *heredoc, int *j)
{
	if (heredoc[data->i + 1] == ' ' || heredoc[data->i + 1] == '\t'
		|| heredoc[data->i + 1] == '\'' || heredoc[data->i + 1] == '\"'
		|| heredoc[data->i + 1] == '\0' || heredoc[data->i + 1] == '\n')
	{
		*j += 1;
		return ;
	}
	else if (heredoc[data->i + 1] == '?')
	{
		var_count_status(j, data);
		*j -= 1;
	}
	else
		var_env_count_init(heredoc, j, data);
}
