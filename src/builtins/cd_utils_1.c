/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:26:07 by acosi             #+#    #+#             */
/*   Updated: 2023/12/10 15:08:24 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Prints an error message if "cd" is invoked with more than one argument.

int	check_cd_args(char **cmd, t_data *data)
{
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	if (i > 2)
	{
		print_error_stderr(NULL, "cd: too many arguments");
		data->status = 1;
	}
	return (i);
}

//	Check for the occurrence of a '~' character in the first argument of "cd".

int	check_tild(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (cmd[0] == '~')
		return (1);
	return (0);
}

/*
	Returns a string containing the path to the home directory.
	If j = 1, it means cd is called without arguments. The 
	environment is parsed to find and return the HOME variable.
	If not present, an error is printed.
	
*/

char	*get_home(char **env, char *cmd, t_data *data, int j)
{
	char	*dir;
	int		i;

	i = 0;
	dir = NULL;
	if (j == 1)
	{
		while (env[i])
		{
			if (ft_strncmp(env[i], "HOME=", 5) == 0)
			{
				dir = ft_strdup(env[i] + 5);
				return (dir);
			}
			i++;
		}
	}
	if (check_tild(cmd) == 1)
	{
		dir = ft_strjoin(data->home, cmd + 1);
		return (dir);
	}
	print_error_stderr(NULL, "cd: HOME not set");
	return (NULL);
}

/*
	Returns the path where "cd" needs to go.
	If used with no arguments or if cmd[1] = '~', returns the home directory.
*/

char	*get_path_cd(char **cmd, char **env, t_data *data)
{
	char	*dir;
	int		i;

	dir = NULL;
	i = 0;
	while (cmd[i])
		i++;
	if (i == 1 || (i == 2 && (check_tild(cmd[1]))))
		dir = get_home(env, cmd[1], data, i);
	else if (i == 2)
		dir = ft_strdup(cmd[1]);
	return (dir);
}

/*
	Prints an error message if the target directory could not be accessed.
	The data->status is used to get the correct error code after exit.
*/

void	error_no_access(char *dir, t_data *data)
{
	print_error_stderr_2("minishell: cd: ", dir, ": No such file or directory");
	data->status = 1;
}
