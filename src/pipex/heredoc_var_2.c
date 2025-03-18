/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_var_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:53:42 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 07:33:55 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_heredoc_var_env(char *str, t_data *data, int pipe_hdc[2])
{
	t_env	*tmp;
	int		start;

	start = 0;
	tmp = data->env_head;
	while (data->env_head)
	{
		if (ft_strncmp(data->env_head->env, str, ft_strlen(str)) == 0)
			break ;
		data->env_head = data->env_head->next;
	}
	if (data->env_head == NULL)
	{
		data->env_head = tmp;
		return ;
	}
	else
	{
		start = ft_strlen(str);
		ft_putstr_fd((data->env_head->env + start), pipe_hdc[1]);
	}
	data->env_head = tmp;
}

void	put_heredoc_var_env_init(t_data *data, char *cmd,
	int pipe_hdc[2], int len)
{
	int		tmp;
	char	*str;

	tmp = data->heredoc_pos;
	data->heredoc_pos++;
	while (cmd[data->heredoc_pos] && cmd[data->heredoc_pos] != ' '
		&& cmd[data->heredoc_pos] != '\t' && cmd[data->heredoc_pos] != '|'
		&& cmd[data->heredoc_pos] != '>' && cmd[data->heredoc_pos] != '<'
		&& cmd[data->heredoc_pos] != '\'' && cmd[data->heredoc_pos] != '\"')
	{
		data->heredoc_pos++;
		len++;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return ;
	len = -1;
	while (tmp < data->heredoc_pos)
		str[++len] = cmd[++tmp];
	str[len] = '=';
	str[len + 1] = '\0';
	fprintf(stderr, "str = %s\n", str);
	put_heredoc_var_env(str, data, pipe_hdc);
	free(str);
}

void	put_heredoc_var_status(t_data *data, int pipe_hdc[2])
{
	int	nb;

	nb = data->status;
	if (nb == 0)
	{
		ft_putchar_fd('0', pipe_hdc[1]);
		return ;
	}
	ft_putchar_fd(data->status, pipe_hdc[1]);
}

void	put_heredoc_var(t_data *data, char *cmd, int pipe_hdc[2])
{
	if (cmd[data->heredoc_pos + 1] == ' '
		|| cmd[data->heredoc_pos + 1] == '\t'
		|| cmd[data->heredoc_pos + 1] == '\0')
	{
		ft_putchar_fd('$', pipe_hdc[1]);
		data->heredoc_pos++;
		return ;
	}
	else if (cmd[data->heredoc_pos + 1] == '?')
	{
		put_heredoc_var_status(data, pipe_hdc);
		data->heredoc_pos += 2;
	}
	else
		put_heredoc_var_env_init(data, cmd, pipe_hdc, 1);
}

void	var_env_replace_heredoc(char *str, int *j, t_data *data, t_struct **itr)
{
	int		pos;
	int		start;
	t_env	*tmp;

	pos = *j;
	start = 0;
	tmp = data->env_head;
	var_env_search_heredoc(data, str);
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
			(*itr)->heredoc->line[pos] = data->env_head->env[start];
			pos++;
			start++;
		}
		*j = pos;
	}
	data->env_head = tmp;
}
