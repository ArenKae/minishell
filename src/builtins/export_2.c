/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 01:51:08 by keschouf          #+#    #+#             */
/*   Updated: 2024/03/28 09:15:40 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**make_env_cpy(t_data *data)
{
	char	**env_cpy;
	int		i;

	i = 0;
	while (data->my_env[i])
		i++;
	env_cpy = malloc(sizeof(char *) * (i + 1));
	if (!env_cpy)
		return (NULL);
	i = 0;
	while (data->my_env[i])
	{
		env_cpy[i] = ft_strdup(data->my_env[i]);
		if (!env_cpy[i])
			return (NULL);
		i++;
	}
	env_cpy[i] = NULL;
	return (env_cpy);
}

void	sort_env_cpy(char **env_cpy)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!env_cpy[i] || !env_cpy[i + 1])
		return ;
	while (env_cpy[i + 1] != NULL)
	{
		if (ft_strcmp(env_cpy[i], env_cpy[i + 1]) > 0)
		{
			tmp = env_cpy[i];
			env_cpy[i] = env_cpy[i + 1];
			env_cpy[i + 1] = tmp;
			i = 0;
		}
		else
			i++;
	}
	i = 0;
}

void	format_string(char **env_cpy, int *flag, int *i, int *j)
{
	ft_putchar_fd(env_cpy[*i][*j], 1);
	ft_putchar_fd('\"', 1);
	*flag += 1;
	*j += 1;
}

void	print_env_cpy(char **env_cpy, int i, int j, int flag)
{
	while (env_cpy[++i])
	{
		j = 0;
		flag = 0;
		if (!ft_strncmp(env_cpy[i], "_=", 2))
			continue ;
		ft_putstr_fd("declare -x ", 1);
		while (env_cpy[i][j])
		{
			if (env_cpy[i][j] == '=' && flag == 0)
				format_string(env_cpy, &flag, &i, &j);
			else
			{
				ft_putchar_fd(env_cpy[i][j], 1);
				j++;
			}
		}
		if (flag == 0)
			ft_putstr_fd("\n", 1);
		else
			ft_putstr_fd("\"\n", 1);
	}
}

void	print_sort_env(t_data *data)
{
	char	**env_cpy;

	if (!data->env_head)
		return ;
	env_cpy = make_env_cpy(data);
	sort_env_cpy(env_cpy);
	print_env_cpy(env_cpy, -1, 0, 0);
	free_tab(env_cpy);
	data->status = 0;
}
