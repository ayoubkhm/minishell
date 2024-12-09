/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 16:25:00 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/09 17:50:18 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_echo(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
	return (0);
}

bool	is_n_option(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (false);
	while (arg[i] && arg[i] == 'n')
		i++;
	if (arg[i] == '\0')
		return (true);
	return (false);
}

int	ft_parsecho(t_cmd_list *list, t_data *data)
{
	bool	newline;
	bool	first_arg;
	int		i;

	(void)data;
	i = 1;
	newline = true;
	first_arg = true;
	while (list->cmd_args && list->cmd_args[i]
		&& is_n_option(list->cmd_args[i]))
	{
		newline = false;
		i++;
	}
	while (list->cmd_args && list->cmd_args[i])
	{
		if (!first_arg)
			write(1, " ", 1);
		printf("%s\n", list->cmd_args[i]);
		first_arg = false;
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}
