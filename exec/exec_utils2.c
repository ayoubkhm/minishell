/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:05:13 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 00:13:09 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_inlist(t_cmd_list *list)
{
	ft_freetab(list->files_list);
	free(list->files_type);
	ft_freetab(list->cmd_args);
	free(list->cmd);
	free(list);
}

char	**ft_updateshlvl(char **tab)
{
	char	**tabret;
	int		i;
	char	*temp;

	i = -1;
	tabret = malloc((ft_tabstrlen(tab) + 1) * sizeof(char *));
	if (!tabret)
		exit(1);
	while (tab[++i])
	{
		if (ft_strncmp(tab[i], "SHLVL=", 6) == 0)
		{
			temp = ft_atoishlvl(tab[i]);
			tabret[i] = ft_strdup(temp);
			free(temp);
		}
		else
			tabret[i] = ft_strdup(tab[i]);
	}
	tabret[i] = NULL;
	return (tabret);
}

char	*ft_atoishlvl(char *str)
{
	char	*newstr;
	char	*temp;
	char	*temp1;
	int		shlvl_value;

	temp1 = ft_copystrfromn(str, 6);
	shlvl_value = ft_atoi(temp1) + 1;
	temp = ft_itoa(shlvl_value);
	free(temp1);
	if (!temp)
		return (NULL);
	newstr = malloc(sizeof(char) * (ft_strlen("SHLVL=") + ft_strlen(temp) + 1));
	if (!newstr)
	{
		free(temp);
		return (NULL);
	}
	ft_strlcpy(newstr, "SHLVL=", 7);
	ft_strlcat(newstr, temp, ft_strlen("SHLVL=") + ft_strlen(temp) + 1);
	free(temp);
	return (newstr);
}

char	*ft_copystrfromn(char *str, int n)
{
	char	*newstr;
	int		len;
	int		i;

	len = ft_strlen(str);
	if (n >= len)
		return (ft_strdup(""));
	newstr = malloc(sizeof(char) * (len - n + 1));
	if (!newstr)
		return (NULL);
	i = 0;
	while (str[n])
		newstr[i++] = str[n++];
	newstr[i] = '\0';
	return (newstr);
}
