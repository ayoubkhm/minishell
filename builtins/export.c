/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:22:10 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/25 16:22:10 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>

char	**ft_export(char **envp, char *str)
{
	char	**tabret;
	int		i;
	int		j;

	tabret = malloc((ft_tabstrlen(envp) + 2) * sizeof(char *));
	if (!tabret)
		exit(1);
	i = -1;
	while (envp[++i])
		tabret[i] = envp[i];
	tabret[i] = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!tabret[i])
		exit(1);
	j = -1;
	while (str[++j])
		tabret[i][j] = str[j];
	tabret[i][j] = '\0';
	tabret[i + 1] = NULL;
	free(envp);
	return (tabret);
}

