/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:06:46 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 04:18:19 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

char	**ft_get_path(char **envp, t_data *data, t_cmd_list *list)
{
	char	**tab;
	int		i;
	char	**split;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (!envp[i])
	{
		write(2,"minishell : No such file or directory\n",39);
		return(data->exit = 127,NULL);
	}
	split = ft_split(&envp[i][5], ':');
	tab = ft_copyntab(split,1);
	if (!tab)
		return (perror("ft_split"),NULL);
	i = 0;
	while(tab[i])
		i++;
	tab[i] = ft_strdup(list->cmd_args[0]);
	tab[i + 1] = NULL;
	ft_freetab(split);
	return (tab);
}
