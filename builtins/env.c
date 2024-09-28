/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 19:45:58 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/23 19:45:58 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!envp)
		return (1);
	while (envp[i])
	{
		j = -1;
		while (envp[i][++j])
			write (1, &envp[i][j], 1);
		write (1, "\n", 1);
		i++;
	}
	return (0);
}
