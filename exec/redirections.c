/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:29:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 18:58:10 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_exec1(t_cmd_list *list, t_data *data, t_env **env_list)
{
	list->open[0] = STDIN_FILENO;
	list->open[1] = STDOUT_FILENO;
	if (ft_openall(list) == -1)
	{
		cleanup_resources(data, env_list, list);
		exit(1);
	}
	if (list->open[0] != STDIN_FILENO)
	{
		dup2(list->open[0], STDIN_FILENO);
		close(list->open[0]);
	}
	if (list->open[1] != STDOUT_FILENO)
	{
		dup2(list->open[1], STDOUT_FILENO);
		close(list->open[1]);
	}
	if (!list->cmd_args[0])
	{
		cleanup_resources(data, env_list, list);
		exit(data->exit);
	}
	return (0);
}

int	ft_exec1par(t_cmd_list *list, t_data *data)
{
	list->open[0] = STDIN_FILENO;
	list->open[1] = STDOUT_FILENO;
	if (ft_openall(list) == -1)
		return (data->exit = 1, 1);
	if (list->open[0] != STDIN_FILENO)
	{
		list->save_std[0] = dup(STDIN_FILENO);
		dup2(list->open[0], STDIN_FILENO);
		close(list->open[0]);
	}
	if (list->open[1] != STDOUT_FILENO)
	{
		list->save_std[1] = dup(STDOUT_FILENO);
		dup2(list->open[1], STDOUT_FILENO);
		close(list->open[1]);
	}
	return (0);
}

int	ft_openall(t_cmd_list *list)
{
	int	i;
	int	fd_in;
	int	fd_out;

	i = 0;
	list->open[0] = STDIN_FILENO;
	list->open[1] = STDOUT_FILENO;
	if (!list->files_list || !list->files_list[0])
		return (0);
	while (list->files_list[i])
	{
		fdinout(list, fd_in, fd_out, i);
		i++;
	}
	return (0);
}

int	fdinout(t_cmd_list *list, int fd_in, int fd_out, int i)
{
	if (list->files_type[i] == 0 || list->files_type[i] == 4)
	{
		fd_in = open(list->files_list[i], O_RDONLY);
	}
	else if (list->files_type[i] == 1)
	{
		fd_out = open(list->files_list[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out == -1)
			return (-1);
		if (list->open[1] != STDOUT_FILENO)
			close(list->open[1]);
		list->open[1] = fd_out;
	}
	else if (list->files_type[i] == 2)
	{
		fd_out = open(list->files_list[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
		if (fd_out == -1)
			return (-1);
		if (list->open[1] != STDOUT_FILENO)
			close(list->open[1]);
		list->open[1] = fd_out;
	}
	return (0);
}
