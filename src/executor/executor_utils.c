/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:13:07 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 07:29:27 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**ft_tab_cpy(char **tab)
{
	int		i;
	char	**cmd;

	i = 0;
	while (tab[i])
		i++;
	cmd = malloc(sizeof(char *) * (i + 1));
	if (!cmd)
		return (NULL);
	cmd[i] = NULL;
	i = 0;
	while (tab[i])
	{
		cmd[i] = ft_strdup(tab[i]);
		i++;
	}
	return (cmd);
}

int	skip_new_line(t_struct **itr, t_data *data)
{
	data->pipes = 0;
	while ((*itr) && (*itr)->is_new_line == 1)
	{
		if ((*itr)->next == NULL)
			return (0);
		*itr = (*itr)->next;
	}
	return (1);
}

int	check_empty_var(t_struct **itr)
{
	if ((*itr)->is_empty_var == 1)
	{
		*itr = (*itr)->next;
		return (1);
	}
	return (0);
}

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
