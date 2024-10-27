/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:44 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:10:59 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	proc_com_args(t_token **tok, t_cmd_list *c_cmd, t_env **e_list, int *arg_i)
{
	char	*expanded_arg;

	if ((*tok)->type == TYPE_WORD
		|| (*tok)->type == TYPE_QUOTED
		|| (*tok)->type == TYPE_ENV_VAR)
	{
		expanded_arg = expand_variables((*tok)->value, *e_list);
		c_cmd->cmd_args[(*arg_i)++] = expanded_arg;
		*tok = (*tok)->next;
		return (1);
	}
	return (0);
}

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

int	process_tokens(t_token **tokens, t_cmd_list *curr_cmd, t_env **env_lis)
{
	int	arg_index;

	arg_index = 0;
	while (*tokens && (*tokens)->type != TYPE_PIPE)
	{
		if (proc_com_args(tokens, curr_cmd, env_lis, &arg_index))
			continue ;
		if (process_redirections(tokens, curr_cmd) == -1)
			return (-1);
		*tokens = (*tokens)->next;
	}
	curr_cmd->cmd_args[arg_index] = NULL;
	return (0);
}

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TYPE_PIPE)
	{
		if (tokens->type == TYPE_WORD || tokens->type == TYPE_QUOTED)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

int	count_arguments(t_token *tokens)
{
	int		arg_count;
	t_token	*tmp;

	arg_count = 0;
	tmp = tokens;
	while (tmp && tmp->type != TYPE_PIPE)
	{
		if (tmp->type == TYPE_WORD
			|| tmp->type == TYPE_QUOTED
			|| tmp->type == TYPE_ENV_VAR)
			arg_count++;
		else if (tmp->type == TYPE_REDIR_IN || tmp->type == TYPE_REDIR_OUT
			|| tmp->type == TYPE_REDIR_APPEND
			|| tmp->type == TYPE_HEREDOC)
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
			continue ;
		}
		tmp = tmp->next;
	}
	return (arg_count);
}
