/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 21:32:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/19 21:32:34 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

long	ft_exatoi(char *str)
{
	long atoi;
	int s;

	atoi = 0;
	s = 1;
	while ((*str >= 9 && *str <= 13) || (*str == ' '))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			s = s * -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		atoi = atoi * 10 + (*str) - 48;
		str++;
		if ((s == 1 && atoi > LLONG_MAX) || (s == -1 && atoi * s < LLONG_MIN))
			return (2);
	}
	if (*str)
		return (2);
	return (atoi * s);
}

int	ft_exit(t_data *data, t_token *token)
{
	int i;

	write(1, "exit\n", 5);
	ft_freedata(data);
	if (token->next)
	{
		i = ft_exatoi(token->next->value);
		free_tokens(token);
		exit(i % 256);
	}
	else
	{
		(void)i;
		free_tokens(token);
		exit(0);
	}
}
