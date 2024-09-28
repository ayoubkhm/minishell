/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:35:58 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/24 15:35:58 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(char **envp, char *str)
{
	int		i;
	int		j;
	char	*target;

	i = 0;
	if (!envp || !str)
		return (1);
	target = ft_addstr(str, "=");
	if (!target)
		return (1);
	while (envp[i] && ft_strncmp(target, envp[i], strlen(target)))
		i++;
	free (target);
	if (!envp[i])
		return (1);
	j = i - 1;
	while (envp[++j])
		envp[j] = envp[j + 1];
	free(envp[j]);
	return (0);
}
