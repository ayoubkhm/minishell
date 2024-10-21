/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 20:20:38 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/23 20:20:38 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_pwd(char **envp)
{
	int	i;
	int	j;

	i = 0;
	if (!envp)
		return (1);
	while (envp[i] && ft_strncmp ("PWD=", envp[i], 4) != 0)
		i++;
	if (!envp[i])
		exit (1);
	j = 3;
	while (envp[i][++j])
		write (1, &envp[i][j], 1);
	write (1, "\n", 1);
	return (0);
}
