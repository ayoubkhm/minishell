/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:43:16 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:43:17 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	process_redirections(t_token **tokens, t_cmd_list *current_cmd)
{
	if ((*tokens)->type == TYPE_REDIR_IN
		|| (*tokens)->type == TYPE_REDIR_OUT
		|| (*tokens)->type == TYPE_REDIR_APPEND)
	{
		if (handle_redirection_w_token(tokens, current_cmd) == -1)
			return (-1);
	}
	return (0);
}

int	process_command_args(t_token **tokens, t_cmd_list *curr_cmd, int *arg_index)
{
	char	*arg_value;

	arg_value = ft_strdup((*tokens)->value);
	if (!arg_value)
		return (-1);
	curr_cmd->cmd_args[(*arg_index)++] = arg_value;
	*tokens = (*tokens)->next;
	return (0);
}

int	process_command_redirections(t_token **tokens, t_cmd_list *curr_cmd)
{
	if ((*tokens)->type == TYPE_HEREDOC)
	{
		if (process_heredoc(tokens, curr_cmd) == -1)
			return (-1);
	}
	else
	{
		if (process_redirections(tokens, curr_cmd) == -1)
			return (-1);
	}
	return (0);
}
