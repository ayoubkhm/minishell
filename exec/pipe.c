/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:30:45 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/19 06:02:29 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int ft_execpipe(t_cmd_list *list, int n)
{
	if(n != 0)
	{
		dup2(list->pipe[2],STDIN_FILENO);
		close(list->pipe[2]);	
	}
	//if(list->pipe)
        	close(list->pipe[0]);
	if (list->next)
		dup2(list->pipe[1], STDOUT_FILENO);
	close(list->pipe[1]);
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

int	ft_execute_child(t_cmd_list *list, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ft_execpipe(list, data->nodenb);
	ft_exec1(list); // Prépare l'exécution
	ft_exec2(list, data, NULL); // Exécute réellement
	return (0);
}