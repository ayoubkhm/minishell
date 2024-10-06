/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:21:10 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/28 20:21:10 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

char	**ft_export(char **envp, char *str)
{
	char	**tabret;
	int		i;
	int		j;

	tabret = malloc((ft_tabstrlen(envp) + 2) * sizeof(char *));
	if (!tabret)
		return(ft_freetab(tabret), NULL);
	i = -1;
	while (envp[++i])
		tabret[i] = ft_strdup(envp[i]);
	tabret[i] = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!tabret[i])
		return(ft_freetab(tabret),NULL);
	j = -1;
	while (str[++j])
		tabret[i][j] = str[j];
	tabret[i][j] = '\0';
	tabret[i + 1] = NULL;
	ft_freetab(envp);
	return (tabret);
}

int	ft_parsexport(t_data *data,t_token *token)
{
	if(!token->next)
		ft_envexport(data->envp);
	if(token->next)
		data->envp = ft_export(data->envp,token->next->value);
	return(0);
}

int	ft_envexport(char **envp)
{
	int	i;
	int	j;

	i = 0;
	if (!envp || !envp[i])
		return (1);
	while (envp[i])
	{
		j = 0;
		if(!envp[i][j])
			break;
		write(1,"export ",7);
		while(envp[i][j] != '=')
		{
			write(1,&envp[i][j],1);
			j++;
		}
		write(1,&envp[i][j],1);
		write(1,"\"",1);
		while(envp[i][j+1] && envp[i][++j])
			write(1,&envp[i][j],1);
		write(1, "\"", 1);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
