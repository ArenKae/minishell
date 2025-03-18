/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:53:25 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/04 16:15:58 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	var_replace_heredoc_status(int *j, t_data *data, t_struct **itr)
{
	int	nb;
	int	pos;

	pos = *j;
	nb = data->status;
	if (nb == 0)
	{
		(*itr)->heredoc->line[pos] = '0';
		j += 1;
		return ;
	}
	if (nb < 0)
	{
		(*itr)->heredoc->line[pos] = '-';
		nb = -nb;
		pos += 1;
	}
	div_and_get_nb(data, &pos, &nb, j);
	while (nb > 0)
	{
		(*itr)->heredoc->line[pos] = (nb % 10) + 48;
		nb = nb / 10;
		pos -= 1;
	}
}

void	var_env_search_heredoc(t_data *data, char *str)
{
	while (data->env_head)
	{
		if (ft_strncmp(data->env_head->env, str, ft_strlen(str)) == 0)
			break ;
		data->env_head = data->env_head->next;
	}
}

void	var_env_replace_heredoc_init(char *heredoc, int *j,
	t_data *data, t_struct **itr)
{
	int		len;
	int		tmp;
	char	*str;

	len = 0;
	tmp = data->pos;
	data->pos++;
	while (heredoc[data->pos] && heredoc[data->pos] != ' '
		&& heredoc[data->pos] != '\t' && heredoc[data->pos] != '$'
		&& heredoc[data->pos] != '\n')
	{
		data->pos++;
		len++;
	}
	str = malloc(sizeof(char) * (len + 2));
	if (!str)
		return ;
	len = -1;
	while (tmp < data->pos)
		str[++len] = heredoc[++tmp];
	str[len] = '=';
	str[len + 1] = '\0';
	var_env_replace_heredoc(str, j, data, itr);
	free(str);
}

void	var_replace_heredoc(char *heredoc, int *j, t_data *data, t_struct **itr)
{
	if (heredoc[data->pos + 1] == ' ' || heredoc[data->pos + 1] == '\t'
		|| heredoc[data->pos + 1] == '\0' || heredoc[data->pos + 1] == '\n')
	{
		(*itr)->heredoc->line[*j] = '$';
		*j += 1;
		return ;
	}
	else if (heredoc[data->pos + 1] == '?')
	{
		var_replace_heredoc_status(j, data, itr);
		data->pos += 1;
		*j += 1;
	}
	else
	{
		var_env_replace_heredoc_init(heredoc, j, data, itr);
		data->pos -= 1;
	}
}

void	make_heredoc_var_count(t_data *data, char *heredoc, int *j)
{
	if (heredoc[data->i + 1] == ' ' || heredoc[data->i + 1] == '\0')
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
