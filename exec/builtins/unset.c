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

#include "builtins.h"

char	**ft_unset(char **envp, char *str)
{
	char	**tabret;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if(!str)
		return(envp);
	str = ft_addstr(str, "=");
	tabret = malloc((ft_tabstrlen(envp)) * sizeof(char *));
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, ft_strlen(str)) != 0)
		{
			tabret[j] = ft_strdup(envp[i]);
			j++;
		}
		i++;
	}
	free(str);
	if(i != j)
		tabret[j] = NULL;
	else
		return(ft_freetabi(tabret,i),envp);
	return (ft_freetab(envp),tabret);
}

int	ft_parsunset(t_data *data, t_cmd_list *list)
{
	data->envp = ft_unset(data->envp,list->cmd_args[1]);
	return (0);
}
