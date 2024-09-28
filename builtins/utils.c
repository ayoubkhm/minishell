/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:20:35 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/24 16:20:35 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_addstr(char *str1, char *str2)
{
	int		i;
	char	*stret;

	stret = malloc((ft_strlen (str1) + ft_strlen (str2) + 1) * sizeof(char));
	if (!stret)
		return (NULL);
	i = -1;
	while (str1[++i])
		stret[i] = str1[i];
	i = -1;
	while (str2[++i])
		stret[i + ft_strlen(str1)] = str2[i];
	stret[i] = '\0';
	return (stret);
}

int	ft_tabstrlen(char **tab)
{
	int	i;

	i = 0;
	if(!tab)
		return (-1);
	while (tab[i])
		i++;
	return (i);
}

char	**ft_copytab(char **tab)
{
	char	**tabret;
	int	i;

	i = -1;
	tabret = malloc((ft_tabstrlen(tab) + 1) * sizeof(char *));
	if(!tabret)
		exit(1);
	while(tab[++i])
		tabret[i] = tab[i];
	return(tabret);
}
