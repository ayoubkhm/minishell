/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:08:44 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/03 21:52:54 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void free_cmd_args(char **cmd_args)
{
    int i = 0;

    if (!cmd_args)
        return;

    while (cmd_args[i])
    {
        free(cmd_args[i]);
        cmd_args[i] = NULL; // Réinitialiser le pointeur après libération
        i++;
    }
    free(cmd_args);
}

void free_files_list(char **files_list, int files_count)
{
    int i = 0;

    if (!files_list)
        return;

    while (i < files_count)
    {
        if (files_list[i])
        {
            free(files_list[i]);
            files_list[i] = NULL; // Réinitialiser après libération
        }
        i++;
    }
    free(files_list);
}


void	free_files_type(int *files_type)
{
	if (files_type)
	{
		free(files_type);
	}
}

void free_cmd_node(t_cmd_list *cmd_node)
{
    if (!cmd_node)
        return;

    free_cmd_args(cmd_node->cmd_args);
    cmd_node->cmd_args = NULL;

    if (cmd_node->cmd)
    {
        free(cmd_node->cmd);
        cmd_node->cmd = NULL;
    }

    free_files_list(cmd_node->files_list, cmd_node->files_count);
    cmd_node->files_list = NULL;

    free_files_type(cmd_node->files_type);
    cmd_node->files_type = NULL;
}





void free_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_list *tmp;

    while (cmd_list->prev != NULL)
	cmd_list = cmd_list->prev;
    while (cmd_list)
    {
        tmp = cmd_list;
        cmd_list = cmd_list->next;
        free_cmd_node(tmp);
        free(tmp);
    }
	// if(cmd_list)
        //         cmd_list = NULL;
}
