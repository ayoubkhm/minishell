/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:29:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/30 00:32:12 by gtraiman         ###   ########.fr       */
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
        exit(g_last_exit_status);
    }
    return (0);
}

int	ft_exec1par(t_cmd_list *list)
{
    list->open[0] = STDIN_FILENO;
    list->open[1] = STDOUT_FILENO;
    if (ft_openall(list) == -1)
        return(g_last_exit_status = 1, 1);
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
    // if (!list->cmd_args[0])
    //     return(g_last_exit_status);
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
        fprintf(stderr, "%d\n", list->files_type[i]);
        if (list->files_type[i] == 0 || list->files_type[i] == 4)
        {
            fprintf(stderr, "%s\n", list->files_list[i]);
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
