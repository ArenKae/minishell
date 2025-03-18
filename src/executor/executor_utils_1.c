/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:13:07 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/30 15:45:37 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	list_command_init(t_chain *chain)
{
	chain->content = NULL;
	chain->is_command = 0;
	chain->is_pipe = 0;
	chain->is_redirect_in = 0;
	chain->is_redirect_out = 0;
	chain->is_append = 0;
	chain->is_heredoc = 0;
	chain->is_empty_var = 0;
	chain->hdc_quotes = 0;
	chain->last_heredoc = 0;
	chain->command_index = 0;
}

t_chain	*make_list_malloc_first(t_chain **head)
{
	t_chain	*chain;

	chain = malloc(sizeof(t_chain));
	if (!chain)
		return (NULL);
	*head = chain;
	return (chain);
}

void	make_list_malloc(t_chain **chain)
{
	(*chain)->next = malloc(sizeof(t_chain));
	if (!(*chain)->next)
		return ;
	(*chain) = (*chain)->next;
}

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
