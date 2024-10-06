/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 16:25:00 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/01 21:10:54 by gtraiman         ###   ########.fr       */
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

int	ft_parsecho(t_token *token)
{
	bool	newline;
	bool	first_arg;

	newline = true;
	first_arg = true;
	// fprintf(stderr,"here\n\n");
	token = token->next;
	while (token && token->value && is_n_option(token->value))
	{
		newline = false;
		token = token->next;
	}
	while (token && token->value)
	{
		if (!first_arg)
			write(1, " ", 1);
		ft_echo(token->value);
		first_arg = false;
		token = token->next;
	}
	if (newline)
		write(1, "\n", 1);
	return (0);
}

