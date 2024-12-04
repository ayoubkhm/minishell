/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 23:19:16 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/04 23:22:27 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	**ft_copyntab(char **tab, int n)
{
	char	**tabret;
	int		i;

	tabret = NULL;
	i = -1;
	tabret = malloc((ft_tabstrlen(tab) + n + 1) * sizeof(char *));
	if (!tabret)
		exit(1);
	while (tab[++i])
		tabret[i] = ft_strdup(tab[i]);
	tabret[i] = NULL;
	return (tabret);
}

void	ft_freetab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

void	ft_freetabi(char **tab, int i)
{
	int	j;

	if (!tab)
		return ;
	j = 0;
	while (j < i)
		free(tab[j++]);
	free(tab);
}

int	ft_printab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		write(1, tab[i], ft_strlen(tab[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

void	ft_freedata(t_data *data)
{
	if (data->av)
		ft_freetab(data->av);
	if (data->envp)
		ft_freetab(data->envp);
	if (data->cwd)
		free(data->cwd);
}
