/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 23:20:44 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/04 23:20:44 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	*ft_addstr(char *str1, char *str2)
{
	int		i;
	char	*stret;

	if (!str1)
		return (str2);
	if (!str2)
		return (str1);
	stret = malloc((ft_strlen(str1) + ft_strlen(str2) + 1) * sizeof(char));
	if (!stret)
		return (NULL);
	i = -1;
	while (str1[++i])
		stret[i] = str1[i];
	i = -1;
	while (str2[++i])
		stret[i + strlen(str1)] = str2[i];
	stret[i + strlen(str1)] = '\0';
	return (stret);
}

int	ft_tabstrlen(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**ft_copytab(char **tab)
{
	char	**tabret;
	int		i;

	i = -1;
	tabret = malloc((ft_tabstrlen(tab) + 1) * sizeof(char *));
	if (!tabret)
		exit(1);
	while (tab[++i])
		tabret[i] = ft_strdup(tab[i]);
	tabret[i] = NULL;
	return (tabret);
}
