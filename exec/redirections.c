/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:29:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/08 03:17:14 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_exec1(t_cmd_list *list, t_data *data, t_env **env_list)
{
	int	test;

	list->open[0] = STDIN_FILENO;
	list->open[1] = STDOUT_FILENO;
	test = ft_openall(list);
	if (test == -2 && !list->cmd_args[0] && (list->next || list->prev))
		return (cleanup_resources(data, env_list, list), exit(0), 0);
	else if (test == -1 || test == -2)
		return (cleanup_resources(data, env_list, list), exit(1), 0);
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
	int	open;

	open = ft_openall(list);
	list->open[0] = STDIN_FILENO;
	list->open[1] = STDOUT_FILENO;
	if (open == -1 || open == -2)
		return (data->exit = 1, 1);
	printf("here\n");
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
	int	test;

	i = 0;
	list->open[0] = STDIN_FILENO;
	list->open[1] = STDOUT_FILENO;
	if (!list->files_list || !list->files_list[0])
		return (0);
	while (list->files_list[i])
	{
		test = fdinout(list, &fd_in, &fd_out, i);
		if (test == -1)
			return (perror("open"), -1);
		if (test == -2)
			return (perror("open"), -2);
		i++;
	}
	return (0);
}

int	fdinout(t_cmd_list *list, int *fd_in, int *fd_out, int i)
{
	if (list->files_type[i] == 0 || list->files_type[i] == 4)
	{
		*fd_in = open(list->files_list[i], O_RDONLY);
		if (*fd_in == -1)
			return (-2);
		if (list->open[0] != STDIN_FILENO)
			close(list->open[0]);
		list->open[0] = *fd_in;
	}
	else if (list->files_type[i] == 1 || list->files_type[i] == 2)
	{
		if (list->files_type[i] == 1)
			*fd_out = open(list->files_list[i], O_WRONLY | O_CREAT | O_TRUNC,
					0666);
		else
			*fd_out = open(list->files_list[i], O_WRONLY | O_CREAT | O_APPEND,
					0666);
		if (*fd_out == -1)
			return (-1);
		if (list->open[1] != STDOUT_FILENO)
			close(list->open[1]);
		list->open[1] = *fd_out;
	}
	return (0);
}
