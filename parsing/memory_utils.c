/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:08:44 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/19 10:44:38 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_cmd_args(char **cmd_args)
{
	int	i;

	i = 0;
	if (cmd_args)
	{
		while (cmd_args[i])
		{
			free(cmd_args[i]);
			i++;
		}
		free(cmd_args);
	}
}

void	free_files_list(char **files_list, int files_count)
{
	int	i;

	i = 0;
	if (files_list)
	{
		while (i < files_count)
		{
			free(files_list[i]);
			i++;
		}
		free(files_list);
	}
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
    free_cmd_args(cmd_node->cmd_args);

    if (cmd_node->cmd)
    {
        free(cmd_node->cmd); // Libération de la commande
        cmd_node->cmd = NULL; // Bonnes pratiques : mettre à NULL après libération
    }

    free_files_list(cmd_node->files_list, cmd_node->files_count);
    free_files_type(cmd_node->files_type);
}


void free_cmd_list(t_cmd_list *cmd_list)
{
    t_cmd_list *tmp;

    while (cmd_list)
    {
        tmp = cmd_list;
        cmd_list = cmd_list->next;
        free_cmd_node(tmp);
        free(tmp);
    }
}
