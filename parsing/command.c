/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 16:40:49 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/08 14:35:11 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd_list	*create_cmd_node(void)
{
	t_cmd_list	*new_node;

	new_node = malloc(sizeof(t_cmd_list));
	if (!new_node)
		return (NULL);
	new_node->save_std[0] = -1;
	new_node->save_std[1] = -1;
	new_node->cmd_args = NULL;
	new_node->cmd = NULL;
	new_node->files_list = NULL;
	new_node->files_type = NULL;
	new_node->files_count = 0;
	new_node->last_in = -1;
	new_node->last_out = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
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
		(*current_cmd)->next->prev = *current_cmd;
		*current_cmd = (*current_cmd)->next;
	}
	(*current_cmd)->last_in = -1;
	(*current_cmd)->last_out = -1;
	return (*current_cmd);
}

void	post_process_command(t_cmd_list *current_cmd, t_env **env_list)
{
	if (current_cmd->cmd_args && current_cmd->cmd_args[0])
	{
		if (current_cmd->cmd)
		{
			free(current_cmd->cmd);
			current_cmd->cmd = NULL;
		}
		current_cmd->cmd = strdup(current_cmd->cmd_args[0]);
		if (ft_strcmp(current_cmd->cmd, "export") == 0)
		{
			handle_export(current_cmd, env_list);
		}
		else if (ft_strcmp(current_cmd->cmd, "unset") == 0)
		{
			handle_unset(current_cmd, env_list);
		}
	}
	remove_empty_arguments(current_cmd);
}

int	p_singl_c(t_cmd_list **cmd_l, t_cmd_list **c_c, t_token **tok, t_env **e_l)
{
	int	arg_count;
	int	ret;

	*c_c = init_command_node(cmd_l, c_c);
	if (!(*c_c))
		return (-1);
	arg_count = count_arguments(*tok);
	if (allocate_command_args(*c_c, arg_count) == -1)
		return (-1);
	ret = process_token_cmd(tok, *c_c);
	if (ret == -1)
		return (-1);
	post_process_command(*c_c, e_l);
	return (0);
}

t_cmd_list	*parse_commands(t_token *tokens, t_env **env_list)
{
	t_cmd_list	*cmd_list;
	t_cmd_list	*current_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (p_singl_c(&cmd_list, &current_cmd, &tokens, env_list) == -1)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		if (tokens && tokens->type == TYPE_PIPE)
			tokens = tokens->next;
	}
	if (current_cmd && current_cmd->cmd)
	{
		free(current_cmd->cmd);
		current_cmd->cmd = NULL;
	}
	return (cmd_list);
}
