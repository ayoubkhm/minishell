/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:40 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:10:48 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	add_to_files_list(t_cmd_list *cmd, char *filename, int new_count)
{
	char	**new_files_list;
	int		i;

	new_files_list = malloc(sizeof(char *) * new_count);
	if (!new_files_list)
		return (-1);
	i = 0;
	while (i < cmd->files_count)
	{
		new_files_list[i] = cmd->files_list[i];
		i++;
	}
	new_files_list[cmd->files_count] = ft_strdup(filename);
	free(cmd->files_list);
	cmd->files_list = new_files_list;
	return (0);
}

int	add_to_files_type(t_cmd_list *cmd, int type, int new_count)
{
	int	*new_files_type;
	int	i;

	new_files_type = malloc(sizeof(int) * new_count);
	if (!new_files_type)
		return (-1);
	i = 0;
	while (i < cmd->files_count)
	{
		new_files_type[i] = cmd->files_type[i];
		i++;
	}
	new_files_type[cmd->files_count] = type;
	free(cmd->files_type);
	cmd->files_type = new_files_type;
	return (0);
}

void	add_redirection(t_cmd_list *cmd, char *filename, int type)
{
	int	new_count;

	new_count = cmd->files_count + 1;
	if (add_to_files_list(cmd, filename, new_count) == -1)
		return ;
	if (add_to_files_type(cmd, type, new_count) == -1)
		return ;
	if (type == 0)
		cmd->last_in = cmd->files_count;
	else if (type == 1 || type == 2)
		cmd->last_out = cmd->files_count;
	cmd->files_count = new_count;
}

int	handle_redirection_w_token(t_token **tokens, t_cmd_list *current_cmd)
{
	int	redir_type;

	if ((*tokens)->type == TYPE_REDIR_IN)
		redir_type = 0;
	else if ((*tokens)->type == TYPE_REDIR_OUT)
		redir_type = 1;
	else if ((*tokens)->type == TYPE_REDIR_APPEND)
		redir_type = 2;
	else
		return (-1);
	*tokens = (*tokens)->next;
	if (*tokens && ((*tokens)->type == TYPE_WORD
			|| (*tokens)->type == TYPE_QUOTED))
	{
		add_redirection(current_cmd, (*tokens)->value, redir_type);
		*tokens = (*tokens)->next;
	}
	else
	{
		return (-1);
	}
	return (0);
}
