/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:29:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/21 19:56:54 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_exec1(t_cmd_list *list)
{
    list->open[0] = STDIN_FILENO;
    list->open[1] = STDOUT_FILENO;
    if (ft_openall(list) == -1)
        exit(1);
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
        exit(g_last_exit_status);
    return (0);
}

int	ft_openall(t_cmd_list *list)
{
    int	i = 0;

    list->open[0] = STDIN_FILENO;
    list->open[1] = STDOUT_FILENO;
    if (!list->files_list || !list->files_list[0])
        return (0);
    while (list->files_list[i])
    {
        if (list->files_type[i] == 0 || list->files_type[i] == 3)
        {
            int fd_in = open(list->files_list[i], O_RDONLY);
            if (fd_in == -1)
                return (-1);
            if (list->open[0] != STDIN_FILENO) 
                close(list->open[0]);
            list->open[0] = fd_in;
        }
        else if (list->files_type[i] == 1)
        {
            int fd_out = open(list->files_list[i], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd_out == -1)
                return (-1);
            if (list->open[1] != STDOUT_FILENO)
                close(list->open[1]);
            list->open[1] = fd_out;
        }
        else if (list->files_type[i] == 2)
        {
            int fd_out = open(list->files_list[i], O_WRONLY | O_CREAT | O_APPEND, 0666);
            if (fd_out == -1)
                return (-1);
            if (list->open[1] != STDOUT_FILENO)
                close(list->open[1]);
            list->open[1] = fd_out;
        }
        i++;
    }
    return (0);
}
