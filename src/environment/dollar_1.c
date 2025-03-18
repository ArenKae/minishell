/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 16:06:26 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 10:18:56 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	var_replace_status(int *j, t_data *data, t_chain *chain)
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

void	put_dollar(t_chain **chain, int *j)
{
	(*chain)->content[*j] = '$';
	*j += 1;
}

/*
	Handles the '$' character.
	If there are two '$', we just print '$$' as an exception.
	If there is nothing after the $, we return with the chain only 
	containing '$' (like in bash : '$' just prints a $).
	If the $ is followed by a '?', we replace this argument by
	the value of the exit status of the last command.
	Else, we replace the argument with the value of the corresponding 
	variable in the environment.
*/

void	replace_var(char *itr_cont, int *j, t_data *data, t_chain **chain)
{
	if (itr_cont[data->pos + 1] == '$')
		return (put_dollar(chain, j));
	if (itr_cont[data->pos + 1] == ' ' || itr_cont[data->pos + 1] == '\t'
		|| itr_cont[data->pos + 1] == '\0' || itr_cont[data->pos + 1] == '\n')
		return (put_dollar(chain, j));
	if (data->open_quote == 0 && (itr_cont[data->pos + 1] == '\''
			|| itr_cont[data->pos + 1] == '\"'))
		return ;
	if (data->open_quote == 1 && (itr_cont[data->pos + 1] == '\''
			|| itr_cont[data->pos + 1] == '\"'))
		return (put_dollar(chain, j));
	else if (itr_cont[data->pos + 1] == '?')
	{
		var_replace_status(j, data, *chain);
		data->pos += 1;
		*j += 1;
	}
	else
	{
		replace_var_env_init(itr_cont, j, data, chain);
		data->pos -= 1;
	}
}
