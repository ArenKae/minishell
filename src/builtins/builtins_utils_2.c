/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 02:52:52 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/20 13:13:25 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*tmp1;
	unsigned char	*tmp2;
	int				i;

	i = 0;
	tmp1 = (unsigned char *)s1;
	tmp2 = (unsigned char *)s2;
	while ((tmp1[i] || tmp2[i]))
	{
		if (tmp1[i] != tmp2[i])
			return (tmp1[i] - tmp2[i]);
		i++;
	}
	return (0);
}

/*
	Get the current working directory of the calling process and stores it 
	in data. Used by pwd builtin. This needs to be stored in case the working 
	directory is removed. This function is also called at the end of the 'cd' 
	builtin, in case it is use between '\n' characters.
*/

void	get_dir(t_data *data)
{
	char	buf[2048];
	char	*dir;

	if (getcwd(buf, 1024))
	{
		free(data->dir);
		dir = getcwd(buf, 2048);
		data->dir = ft_strdup(dir);
	}
}

//	Creates a new env in case it has been completely unset.

void	update_env(t_data *data, t_env **new_node, char *var)
{
	(*new_node) = add_env_node(var);
	data->env_head = (*new_node);
	free(data->my_env);
	malloc_env_array(data, data->env_head);
}
