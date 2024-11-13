/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:30:45 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/13 20:43:24 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_execpipe(t_cmd_list *list,t_data *data)
{
	(void)data;
	if(!list->next)
		return(0);
	if(list->pipe[0])
	{
		dup2(list->pipe[0], STDIN_FILENO);
		close(list->pipe[0]);
	}
	if(list->next)
	{
		if (pipe(list->pipe) == -1)
		{
			perror("pipe");
			return (-1);
		}
		return (0);
		
	}
	return(0);
	
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