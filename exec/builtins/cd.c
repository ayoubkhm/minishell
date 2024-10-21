/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/10/09 21:56:56 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/09 21:56:56 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"


int	ft_cd(t_token *token, t_data *data)
{
	char *path;

	path = token->next->value;
	if (path == NULL || strlen(path) == 0)
		return (1);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	if (data->cwd)
		free(data->cwd);
	data->cwd = malloc(4096);
	if (!data->cwd)
		return (1);
	if (getcwd(data->cwd, 1024) == NULL)
	{
		perror("getcwd");
		free(data->cwd);
		data->cwd = NULL;
		return (1);
	}
	ft_update_env(data, "PWD");
	ft_update_env(data, "OLDPWD");
	return (0);
}

int	ft_update_env(t_data *data, char *str)
{
	int i;
	char *new_value;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], str,
			ft_strlen(str)) != 0)
		i++;
	if (!data->envp[i])
		return (-1);
	ft_delfromc(data->envp[i], '=');
	new_value = ft_addstr(data->envp[i], data->cwd);
	if (!new_value)
		return (-1);
	free(data->envp[i]);
	data->envp[i] = new_value;
	return (0);
}
