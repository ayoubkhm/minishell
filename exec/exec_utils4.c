/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:06:46 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 00:13:03 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

char	**ft_get_path(char **envp, t_data *data)
{
	char	**tab;
	int		i;
	char	**split;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
	{
		i++;
	}
	if (!envp[i])
	{
		write(2,"minishell : No such file or directory\n",39);
		data->exit = 127;
		return(NULL);
	}
	split = ft_split(&envp[i][5], ':');
	tab = ft_copyntab(split,1);
	if (!tab)
	{
		perror("ft_split");
		return (NULL);
	}
	i = 0;
	while(tab[i])
		i++;
	tab[i] = malloc(1024);
	if (!tab[i])
	{
		perror("malloc");
		return (NULL);
	}
	if (!getcwd(tab[i], 1024))
	{
		perror("getcwd");
		free(tab[i]);
		tab[i] = NULL;
	}
	tab[i + 1] = NULL;
	ft_freetab(split);
	return (tab);
}
