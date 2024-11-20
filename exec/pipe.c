/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:30:45 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/20 10:25:35 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int ft_execpipe(t_cmd_list *list)
{
	if(list->prev)
	{
		dup2(list->prev->pipe[0],STDIN_FILENO);
		close(list->prev->pipe[0]);
	}
	if (list->next)
	{
		dup2(list->pipe[1], STDOUT_FILENO);
		close(list->pipe[1]);
        	close(list->pipe[0]);
	}
	return (0);
}

int	makeapipe(int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	return (0);
}

void	ft_close_pipes(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
}

int	ft_execute_child(t_cmd_list *list, t_data *data, t_env **env_list)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ft_execpipe(list);
	ft_exec1(list); // Prépare l'exécution
	ft_exec2(list, data, env_list); // Exécute réellement
	return (0);
}