/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:33:01 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 11:03:44 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	This function tries to change the current directory with the path pointed
	by "dir". If chdir() fails to access the target directory (for example if 
	the user doesn't have sufficient permissions), an error message is printed.
*/

void	do_chdir(char *dir, t_data *data)
{
	if (chdir(dir) == -1)
	{
		data->status = 1;
		print_error_stderr(NULL, "cd: ");
		perror(dir);
	}
}

/*
	Util function for check_unset_pwd(). Takes care of the realloc
	of the OLDPWD environment variable stored in the t_env linked list.
*/

void	empty_oldpwd(t_env *env_list, int *flag)
{
	t_env	*env_tmp;

	env_tmp = env_list;
	while (env_tmp)
	{
		if (*flag == 0)
		{
			if (ft_strncmp(env_tmp->env, "OLDPWD=", 7) == 0)
			{
				free(env_tmp->env);
				env_tmp->env = ft_strdup("OLDPWD=");
				return ;
			}
		}
		env_tmp = env_tmp->next;
	}
}

/*
	In case the PWD environment variable has been unset, this function 
	will realloc the OLDPWD variable and empty its content, to replicate 
	the behavior of bash.
*/

void	check_unset_pwd(t_env *env_list)
{
	t_env	*env_tmp;
	int		flag;

	env_tmp = env_list;
	while (env_tmp)
	{
		if (ft_strncmp(env_tmp->env, "PWD=", 4))
			flag = 0;
		else if (ft_strncmp(env_tmp->env, "PWD=", 4) == 0)
		{
			flag = 1;
			break ;
		}
		env_tmp = env_tmp->next;
	}
	empty_oldpwd(env_tmp, &flag);
}

/*
	If getcwd returns NULL after trying to change the working directory,
	it means the dir was removed while the process was inside of it.
	To avoid a segfault and to update PWD and OLPWD accordingly, this 
	function joins the last working directory with the relative path 
	that was used by cd.
*/

char	*invalid_path(t_data *data, t_env *env, char *path)
{
	char	*dir;
	char	*tmp;

	dir = NULL;
	print_error_stderr_2("cd: error retrieving current directory: getcwd: ",
		"cannot access parent directories: ", "No such file or directory");
	while (env)
	{
		if (ft_strncmp(env->env, "PWD=", 4) == 0)
		{
			tmp = ft_strjoin(env->env + 4, "/");
			dir = ft_strjoin(tmp, path);
			free(tmp);
			break ;
		}
		env = env->next;
	}
	free(data->dir);
	data->dir = ft_strdup(dir);
	return (dir);
}
