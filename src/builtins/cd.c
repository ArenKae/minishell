/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 02:54:54 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 13:08:07 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Realloc the content of the "OLPDWD" environment variable 
	after the current directory was changed. var corresponds
	to the string literal "OLDPWD=".
*/

void	redefine_old_pwd(t_data *data, t_env *env_list, char *var)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->env, var, 7) == 0)
		{
			free(env_list->env);
			env_list->env = ft_strjoin("OLDPWD=", data->oldpwd);
			free(data->oldpwd);
			break ;
		}
		env_list = env_list->next;
	}
}

/*
	Copies and stores the content of the current PWD environment 
	variable in the struct data. This is useful to redefine "OLPDWD" 
	environment varaible when the current directory changed.
*/

void	copy_old_pwd(t_data *data, char *pwd)
{
	int	i;
	int	j;
	int	len;

	i = -1;
	j = 3;
	len = ft_strlen(pwd) - 3;
	free(data->oldpwd);
	data->oldpwd = malloc(sizeof(char) * (len + 1));
	if (data->oldpwd == NULL)
		return ;
	while (pwd && ++i < len)
		data->oldpwd[i] = pwd[++j];
	data->oldpwd[i] = '\0';
}

/*
	Compares the current node from the env_list with the string literal "PWD=".
	If an occurrence is found, the content of the current PWD variable is used 
	stored to define the "OLDPWD" variable later.
*/

int	redefine_pwd(t_data *data, t_env *env_list, char *pwd, char *dir)
{
	if (ft_strncmp(env_list->env, pwd, 4) == 0)
	{
		copy_old_pwd(data, env_list->env);
		free(env_list->env);
		env_list->env = ft_strjoin("PWD=", dir);
		return (1);
	}
	return (0);
}

void	redefine_env(t_data *data, t_env *head, char *path)
{
	int		free_flag;
	char	*dir;
	char	buf[2048];

	free_flag = 0;
	dir = getcwd(buf, 2048);
	if (dir == NULL)
	{
		dir = invalid_path(data, data->env_head, path);
		free_flag = 1;
	}
	while (data->env_head)
	{
		if (redefine_pwd(data, data->env_head, "PWD=", dir))
		{
			redefine_old_pwd(data, head, "OLDPWD=");
			break ;
		}
		data->env_head = data->env_head->next;
	}
	if (free_flag == 1)
		free(dir);
}

/*
	Main cd builtin. First checks for invalid arguments and stores the 
	path of the target directory in "dir". If dir can be accessed, the 
	chdir() function will change it to be the current directory. If the 
	"cd" command is piped, it should not do anything, hence the return. 
	After the current directory is changed with chdir(), the "PWD" and 
	"OLDPWD" environment variables are redefined. Finally, we check if 
	the "PWD" variable was unset earlier, in which case "OLDPWD" should 
	be emptied.
*/

void	cd(char **cmd, t_data *data, char **env)
{
	char	*dir;
	t_env	*head;
	t_env	*head_restore;

	if (check_cd_args(cmd, data) > 2)
		return ;
	dir = get_path_cd(cmd, env, data);
	if (dir == NULL)
		return ;
	if (access(dir, F_OK) == 0)
	{
		do_chdir(dir, data);
		if (data->pipes > 0)
			return (free(dir));
	}
	else if (access(dir, F_OK) != 0)
		error_no_access(dir, data);
	free(dir);
	head = data->env_head;
	head_restore = data->env_head;
	redefine_env(data, head, cmd[1]);
	check_unset_pwd(head);
	data->env_head = head_restore;
	data->oldpwd = NULL;
	get_dir(data);
}
