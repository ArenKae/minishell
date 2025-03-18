/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 18:50:58 by acosi             #+#    #+#             */
/*   Updated: 2023/12/02 03:30:49 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_cmd(t_chain **itr, char **cmd, int *j)
{
	if ((*itr)->is_redirect_in != 1 && (*itr)->is_redirect_out != 1
		&& (*itr)->is_append != 1 && (*itr)->is_heredoc != 1)
	{
		cmd[*j] = (*itr)->content;
		*j += 1;
	}
	*itr = (*itr)->next;
}
