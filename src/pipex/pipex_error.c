/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:23:31 by acosi             #+#    #+#             */
/*   Updated: 2023/11/27 18:05:09 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_size2(char const *s1, char const *s2)
{
	int	i;
	int	j;

	i = 0;
	while (s1[i])
		i++;
	j = i;
	i = 0;
	while (s2[i])
		i++;
	j = j + i;
	return (j);
}

char	*ft_strjoin2(char *s1, char const *s2)
{
	char	*str;
	int		i;
	int		j;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = -1;
	str = malloc(sizeof(char) * (get_size2(s1, s2) + 2));
	if (!str)
		return (NULL);
	while (s1[++i])
		str[i] = s1[i];
	free(s1);
	j = i;
	i = -1;
	str[j] = ' ';
	j++;
	while (s2[++i])
	{
		str[j] = s2[i];
		j++;
	}
	str[j] = '\0';
	return (str);
}

//	Check if there is a pipe before any argument.

int	check_pipe_error(t_struct *itr)
{
	if (itr->content[0] == '|')
	{
		print_error_stderr(NULL, "parse error near `|'");
		return (1);
	}
	return (0);
}

//	Check if there is an unclosed pipe at the end of the line.

int	check_unclosed_pipe(t_struct *itr)
{
	while (itr && itr->is_new_line != 1)
	{
		if (itr->is_pipe == 1 && itr->next == NULL)
			return (1);
		itr = itr->next;
	}
	return (0);
}

//	If there is an unclosed pipe, display the prompt again and 
//	parses the new line to add it to the linked list.

void	unclosed_pipe(t_struct *itr, t_data *data, t_struct **head)
{
	char	*cmd;

	while (check_unclosed_pipe(itr))
	{
		cmd = readline("> ");
		if (cmd == NULL)
		{
			free(data->my_env);
			free_env(data->env_head);
			free_list(*head);
			free(data->cmd);
			rl_clear_history();
			exit(0);
		}
		if (cmd != NULL && cmd[0] != '\0' && cmd[0] != '\n')
			data->full_cmd = ft_strjoin2(data->full_cmd, cmd);
		unclosed_pipe_parsing(itr, cmd, data);
		free(cmd);
	}
	add_history(data->full_cmd);
}
