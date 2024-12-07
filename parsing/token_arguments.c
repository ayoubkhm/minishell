/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:06:37 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:06:38 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	proc_com_args(t_token **tok, t_cmd_list *c_cmd, int *arg_i)
{
	if ((*tok)->type == TYPE_WORD
		|| (*tok)->type == TYPE_QUOTED || (*tok)->type == TYPE_ENV_VAR)
	{
		c_cmd->cmd_args[(*arg_i)++] = ft_strdup((*tok)->value);
		*tok = (*tok)->next;
		return (1);
	}
	return (0);
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
