/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:06:46 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/07 22:25:51 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		write(2, "minishell : No such file or directory\n", 39);
		return (data->exit = 127, NULL);
	}
	split = ft_split(&envp[i][5], ':');
	tab = ft_copyntab(split, 1);
	if (!tab)
		return (perror("ft_split"), NULL);
	i = 0;
	while (tab[i])
		i++;
	tab[i] = ft_strdup(list->cmd_args[0]);
	tab[i + 1] = NULL;
	ft_freetab(split);
	return (tab);
}

char	*ft_get_command_path(char *cmd, t_data *data, t_cmd_list *list)
{
	char	*path;
	char	**tab;

	path = NULL;
	if (!data->envp)
		return (NULL);
	tab = ft_get_path(data->envp, data, list);
	if (!tab)
		return (ft_freetab(tab), NULL);
	if (ft_access(tab, cmd, &path) == -1)
		return (ft_freetab(tab), NULL);
	ft_freetab(tab);
	return (path);
}

void	ft_waitall(t_data *data, pid_t last_pid)
{
	int		status;
	pid_t	current_pid;

	current_pid = waitpid(-1, &status, 0);
	while (current_pid > 0)
	{
		if (current_pid == last_pid && WIFEXITED(status))
			data->exit = WEXITSTATUS(status);
		if (current_pid == last_pid && WIFSIGNALED(status))
			data->exit = 128 + WTERMSIG(status);
		current_pid = waitpid(-1, &status, 0);
	}
}

void	initpipe(t_cmd_list *list)
{
	while (list)
	{
		list->pipe[0] = 0;
		list->pipe[1] = 0;
		list = list->next;
	}
}
