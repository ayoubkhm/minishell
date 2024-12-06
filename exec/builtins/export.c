/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
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
		return (ft_freetab(tabret), NULL);
	i = -1;
	while (envp[++i])
		tabret[i] = ft_strdup(envp[i]);
	tabret[i] = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!tabret[i])
		return (ft_freetab(tabret), NULL);
	j = -1;
	while (str[++j])
		tabret[i][j] = str[j];
	tabret[i][j] = '\0';
	tabret[i + 1] = NULL;
	ft_freetab(envp);
	return (tabret);
}

int	ft_parsexport(t_data *data, t_cmd_list *list)
{
	int	i;

	i = 1;
	if (!list->cmd_args[1])
		ft_envexport(data->envp);
	while (list->cmd_args[i] && list && ft_checkexport(list->cmd_args[i]) == 0)
	{
		if (ft_dblexport(data->envp, list->cmd_args[i]) == 0)
			data->envp = ft_export(data->envp, list->cmd_args[i]);
		i++;
	}
	if (list->cmd_args[i])
	{
		write(2, "export : `", 11);
		ft_putstr_fd(list->cmd_args[i], 2);
		write(2, "': not a valid identifier\n", 27);
		data->exit = 1;
		return(1);
	}
	return (0);
}

int	ft_dblexport(char **envp, char *str)
{
	int	i;
	int	c;

	if (!envp || !str)
		return (0);
	i = 0;
	c = ft_strlkforc(str, '=');
	while (envp[i])
	{
		if (ft_strncmp(envp[i], str, c) == 0 && envp[i][c] == '=')
		{
			free(envp[i]);
			envp[i] = ft_strdup(str);
			if (!envp[i])
				return (-1);
			return (1);
		}
		i++;
	}
	return (0);
}

// int	ft_envexport(char **envp)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	if (!envp || !envp[i])
// 		return (1);
// 	while (envp[i])
// 	{
// 		if (!envp[i][0])
// 			break ;
// 		write(1, "export ", 7);
// 		j = -1;
// 		while (envp[i][++j] != '=' && envp[i][j])
// 			write(1, &envp[i][j], 1);
// 		if (envp[i][j])/// erreur export ""=4, signal 1
// 		{
// 			write(1, &envp[i][j], 1);
// 			write(1, "\"", 1);
// 			while (envp[i][j + 1] && envp[i][++j])
// 				write(1, &envp[i][j], 1);
// 			write(1, "\"", 1);
// 		}
// 		i++;
// 		write(1, "\n", 1);
// 	}
// 	return (0);
// }

int	ft_envexport(char **envp)
{
	(void)envp;
	printf("Never gonna give you up\n");
	printf("Never gonna let you dowwwnnnn\n");
	return (0);
}
