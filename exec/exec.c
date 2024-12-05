/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:31:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 21:44:40 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec(t_cmd_list *list, t_data *data, t_env **env_list)
{
	if (!list)
		return (data->exit);
	if (!list->next && !list->prev)
	{
		if (checkbi(list) == 0)
		{
			ft_exec1par(list, data);
			ft_resbi(list, data, env_list, 0);
		}
		else
			ft_exechild(list, data, env_list);
	}
	else
		ft_exechild(list, data, env_list);
	return (0);
}

int	ft_exechild(t_cmd_list *list, t_data *data, t_env **env_list)
{
	pid_t	pid;
	int		lpid;

	if (list->next)
		makeapipe(list->pipe);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
		ft_execinchild(list, data, env_list);
	if (pid > 0)
	{
		lpid = pid;
		if (list->prev)
			close(list->prev->pipe[0]);
		if (list->next)
		{
			close(list->pipe[1]);
			ft_exec(list->next, data, env_list);
		}
		else
			ft_waitall(data, lpid);
	}
	return (0);
}

void	ft_execinchild(t_cmd_list *list, t_data *data, t_env **env_list)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (list->next || list->prev)
		ft_execpipe(list);
	ft_exec1(list, data, env_list);
	if (checkbi(list) == 0)
		ft_resbi(list, data, env_list, 1);
	else
		ft_exec2(list, data, env_list);
}

int	ft_exec2(t_cmd_list *list, t_data *data, t_env **env_list)
{
	char	*path;

	path = ft_get_command_path(list->cmd_args[0], data, list);
	if (!path || path == NULL)
	{
		free(path);
		cleanup_resources(data, env_list, list);
		data->exit = 127;
		exit(data->exit);
	}
	if (execve(path, list->cmd_args, data->envp) == -1)
	{
		perror("execve");
		free(path);
		cleanup_resources(data, env_list, list);
		data->exit = 1;
		exit(data->exit);
	}
	return (0);
}
