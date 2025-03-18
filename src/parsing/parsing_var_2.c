/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:16:05 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/24 16:02:14 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	var_replace_status(int *j, t_data *data, t_struct *itr)
{
	int	nb;
	int	pos;

	*j += 1;
	pos = *j;
	nb = data->status;
	if (nb == 0)
	{
		itr->content[pos] = '0';
		j += 1;
		return ;
	}
	if (nb < 0)
	{
		itr->content[pos] = '-';
		nb = -nb;
		pos += 1;
	}
	div_and_get_nb(data, &pos, &nb, j);
	while (nb > 0)
	{
		itr->content[pos] = (nb % 10) + 48;
		nb = nb / 10;
		pos -= 1;
	}
}

void	var_env_replace_2(t_data *data, char *str)
{
	while (data->env_head)
	{
		if (ft_strncmp(data->env_head->env, str, ft_strlen(str)) == 0)
			break ;
		data->env_head = data->env_head->next;
	}
}

void	var_env_replace(char *str, int *j, t_data *data, t_struct *itr)
{
	int		pos;
	int		start;
	t_env	*tmp;

	*j += 1;
	pos = *j;
	tmp = data->env_head;
	var_env_replace_2(data, str);
	if (data->env_head == NULL)
	{
		data->pos--;
		data->env_head = tmp;
		return ;
	}
	else
	{
		start = ft_strlen(str) - 1;
		while (data->env_head->env[++start])
		{
			itr->content[pos] = data->env_head->env[start];
			pos++;
		}
		*j = pos;
	}
	data->env_head = tmp;
}

void	var_env_replace_init(char *cmd, int *j, t_data *data, t_struct *itr)
{
	int		len;
	int		tmp;
	char	*str;

	len = 1;
	tmp = data->pos;
	data->pos++;
	while (cmd[data->pos] && cmd[data->pos] != ' '
		&& cmd[data->pos] != '\t' && cmd[data->pos] != '|'
		&& cmd[data->pos] != '>' && cmd[data->pos] != '<'
		&& cmd[data->pos] != '$' && cmd[data->pos] != '\"')
	{
		data->pos++;
		len++;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return ;
	len = -1;
	while (tmp < data->pos)
		str[++len] = cmd[++tmp];
	str[len] = '=';
	str[len + 1] = '\0';
	var_env_replace(str, j, data, itr);
	free(str);
}

void	var_replace(char *cmd, int *j, t_data *data, t_struct *itr)
{
	if (cmd[data->pos + 1] == ' ' || cmd[data->pos + 1] == '\t'
		|| cmd[data->pos + 1] == '\0' || cmd[data->pos + 1] == '\"')
	{
		*j += 1;
		itr->content[*j] = '$';
		return ;
	}
	if (cmd[data->pos + 1] == '_' && cmd[data->pos + 2] == '\0')
		itr->is_var = 1;
	if (cmd[data->pos + 1] == '?')
	{
		var_replace_status(j, data, itr);
		data->pos += 1;
	}
	else
	{
		var_env_replace_init(cmd, j, data, itr);
		*j -= 1;
		data->pos--;
	}
}
