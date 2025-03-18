/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:09:37 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/13 01:31:39 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	div_and_get_nb(t_data *data, int *pos, int *nb, int *j)
{
	while (*nb > 9)
	{
		*nb = *nb / 10;
		*pos += 1;
	}
	*j = *pos;
	*nb = data->status;
}

void	free_list(t_struct *head)
{
	t_struct	*tmp;
	t_heredoc	*hdc_tmp;

	while (head)
	{
		tmp = head->next;
		if (head->is_heredoc == 1 && head->content != NULL)
		{
			while (head->heredoc)
			{
				hdc_tmp = head->heredoc->next;
				free(head->heredoc->line);
				free(head->heredoc);
				head->heredoc = hdc_tmp;
			}
		}
		free(head->content);
		free(head);
		head = tmp;
	}
}

void	free_chain(t_chain *head)
{
	t_chain	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->content);
		free(head);
		head = tmp;
	}
}

void	free_env(t_env *head)
{
	t_env	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->env);
		free(head);
		head = tmp;
	}
}
