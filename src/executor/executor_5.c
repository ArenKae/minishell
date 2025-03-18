/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 16:06:26 by acosi             #+#    #+#             */
/*   Updated: 2023/11/28 16:06:38 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	var_replace_chain_status(int *j, t_data *data, t_chain *chain)
{
	int	nb;
	int	pos;

	pos = *j;
	nb = data->status;
	if (nb == 0)
	{
		chain->content[pos] = '0';
		j += 1;
		return ;
	}
	if (nb < 0)
	{
		chain->content[pos] = '-';
		nb = -nb;
		pos += 1;
	}
	div_and_get_nb(data, &pos, &nb, j);
	while (nb > 0)
	{
		chain->content[pos] = (nb % 10) + 48;
		nb = nb / 10;
		pos -= 1;
	}
}

void	var_env_search_chain(t_data *data, char *str)
{
	while (data->env_head)
	{
		if (ft_strncmp(data->env_head->env, str, ft_strlen(str)) == 0)
			break ;
		data->env_head = data->env_head->next;
	}
}

void	var_env_replace_chain(char *str, int *j, t_data *data, t_chain *chain)
{
	int		pos;
	int		start;
	t_env	*tmp;

	pos = *j;
	start = 0;
	tmp = data->env_head;
	var_env_search_chain(data, str);
	if (data->env_head == NULL)
	{
		data->env_head = tmp;
		return ;
	}
	else
	{
		start = ft_strlen(str);
		while (data->env_head->env[start])
		{
			chain->content[pos] = data->env_head->env[start];
			pos++;
			start++;
		}
		*j = pos;
	}
	data->env_head = tmp;
}

void	var_env_replace_chain_init(char *itr_cont, int *j,
	t_data *data, t_chain *chain)
{
	int		len;
	int		tmp;
	char	*str;

	len = 0;
	tmp = data->pos;
	data->pos++;
	while (itr_cont[data->pos] && itr_cont[data->pos] != ' '
		&& itr_cont[data->pos] != '\t' && itr_cont[data->pos] != '$')
	{
		data->pos++;
		len++;
	}
	str = malloc(sizeof(char) * (len + 2));
	if (!str)
		return ;
	len = -1;
	while (tmp < data->pos)
		str[++len] = itr_cont[++tmp];
	str[len] = '=';
	str[len + 1] = '\0';
	var_env_replace_chain(str, j, data, chain);
	free(str);
}

void	var_replace_chain(char *itr_cont, int *j, t_data *data, t_chain *chain)
{
	if (itr_cont[data->pos + 1] == ' ' || itr_cont[data->pos + 1] == '\t'
		|| itr_cont[data->pos + 1] == '\0' || itr_cont[data->pos + 1] == '\n')
	{
		chain->content[*j] = '$';
		*j += 1;
		return ;
	}
	else if (itr_cont[data->pos + 1] == '?')
	{
		var_replace_chain_status(j, data, chain);
		data->pos += 1;
		*j += 1;
	}
	else
	{
		var_env_replace_chain_init(itr_cont, j, data, chain);
		data->pos -= 1;
	}
}
