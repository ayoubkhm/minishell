/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execbi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 18:36:39 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 22:19:51 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parsebi(t_cmd_list *list, t_data *data, t_env **env_list)
{
	if (ft_strcmp(list->cmd_args[0], "cd") == 0)
		return (ft_cd(list, data));
	if (ft_strcmp(list->cmd_args[0], "echo") == 0)
		return (ft_parsecho(list, data));
	if (ft_strcmp(list->cmd_args[0], "env") == 0)
		return (ft_env(data->envp));
	if (ft_strcmp(list->cmd_args[0], "exit") == 0)
		ft_exit(data, env_list, list);
	if (ft_strcmp(list->cmd_args[0], "pwd") == 0)
		return (ft_pwd(data, list));
	if (ft_strcmp(list->cmd_args[0], "unset") == 0)
		return (ft_parsunset(data, list));
	if (ft_strcmp(list->cmd_args[0], "export") == 0)
		return (ft_parsexport(data, list));
	return (256);
}

int	checkbi(t_cmd_list *list)
{
	if (!list->cmd_args[0])
		return (1);
	// if (ft_strcmp(list->cmd_args[0], "[") == 0)
	// 	return (0);
	if (ft_strcmp(list->cmd_args[0], ":") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "!") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "cd") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "echo") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "env") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "exit") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "pwd") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "unset") == 0)
		return (0);
	if (ft_strcmp(list->cmd_args[0], "export") == 0)
		return (0);
	return (256);
}

int	ft_resbi(t_cmd_list *list, t_data *data, t_env **env_list, int i)
{
	int	resbi;

	resbi = parsebi(list, data, env_list);
	if (list->save_std[0] != -1)
	{
		dup2(list->save_std[0], STDIN_FILENO);
		close(list->save_std[0]);
		list->save_std[0] = -1;
	}
	if (list->save_std[1] != -1)
	{
		dup2(list->save_std[1], STDOUT_FILENO);
		close(list->save_std[1]);
		list->save_std[1] = -1;
	}
	if (resbi != 256)
	{
		data->exit = resbi;
		if (i == 0)
			return (data->exit);
		if (i == 1)
			return (cleanup_resources(data, env_list, list), exit(data->exit),
				0);
	}
	return (0);
}
