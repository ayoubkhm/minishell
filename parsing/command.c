/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:09:19 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:09:34 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd_list	*create_cmd_node(void)
{
	t_cmd_list	*new_node;

	new_node = malloc(sizeof(t_cmd_list));
	if (!new_node)
		return (NULL);
	new_node->cmd_args = NULL;
	new_node->cmd = NULL;
	new_node->files_list = NULL;
	new_node->files_type = NULL;
	new_node->files_count = 0;
	new_node->last_in = -1;
	new_node->last_out = -1;
	new_node->next = NULL;
	return (new_node);
}

t_cmd_list	*init_command_node(t_cmd_list **cmd_list, t_cmd_list **current_cmd)
{
	if (!(*cmd_list))
	{
		*cmd_list = create_cmd_node();
		*current_cmd = *cmd_list;
	}
	else
	{
		(*current_cmd)->next = create_cmd_node();
		*current_cmd = (*current_cmd)->next;
	}
	(*current_cmd)->last_in = -1;
	(*current_cmd)->last_out = -1;
	return (*current_cmd);
}

int	allocate_command_args(t_cmd_list *current_cmd, int arg_count)
{
	current_cmd->cmd_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!current_cmd->cmd_args)
		return (-1);
	return (0);
}

void	post_process_command(t_cmd_list *current_cmd, t_env **env_list)
{
	if (current_cmd->cmd_args && current_cmd->cmd_args[0])
	{
		current_cmd->cmd = strdup(current_cmd->cmd_args[0]);
		if (strcmp(current_cmd->cmd, "export") == 0)
		{
			handle_export(current_cmd, env_list);
		}
	}
}

t_cmd_list	*parse_commands(t_token *tokens, t_env **env_list)
{
	t_cmd_list	*cmd_list;
	t_cmd_list	*current_cmd;
	int			arg_count;

	cmd_list = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		current_cmd = init_command_node(&cmd_list, &current_cmd);
		arg_count = count_arguments(tokens);
		if (allocate_command_args(current_cmd, arg_count) == -1)
			return (NULL);
		if (process_tokens(&tokens, current_cmd, env_list) == -1)
			return (NULL);
		post_process_command(current_cmd, env_list);
		if (tokens && tokens->type == TYPE_PIPE)
			tokens = tokens->next;
	}
	return (cmd_list);
}
