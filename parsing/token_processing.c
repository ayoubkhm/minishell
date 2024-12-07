/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:06:19 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:06:20 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	skip_token(t_token **tokens)
{
	*tokens = (*tokens)->next;
}

int	handle_argument_tokens(t_token **tok, t_cmd_list *curr_cmd, int *arg_index)
{
	if (process_command_args(tok, curr_cmd, arg_index) == -1)
		return (-1);
	return (0);
}

int	handle_redirection_tokens(t_token **tokens, t_cmd_list *curr_cmd)
{
	if (process_command_redirections(tokens, curr_cmd) == -1)
		return (-1);
	return (0);
}

int	process_token_cmd(t_token **tokens, t_cmd_list *curr_cmd)
{
	int	arg_index;

	arg_index = 0;
	while (*tokens && (*tokens)->type != TYPE_PIPE)
	{
		if ((*tokens)->type == TYPE_WORD
			|| (*tokens)->type == TYPE_QUOTED
			|| (*tokens)->type == TYPE_ENV_VAR)
		{
			if (handle_argument_tokens(tokens, curr_cmd, &arg_index) == -1)
				return (-1);
		}
		else if ((*tokens)->type == TYPE_REDIR_IN
			|| (*tokens)->type == TYPE_REDIR_OUT
			|| (*tokens)->type == TYPE_REDIR_APPEND
			|| (*tokens)->type == TYPE_HEREDOC)
		{
			if (handle_redirection_tokens(tokens, curr_cmd) == -1)
				return (-1);
		}
		else
			skip_token(tokens);
	}
	curr_cmd->cmd_args[arg_index] = NULL;
	return (0);
}
