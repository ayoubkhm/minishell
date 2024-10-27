/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_syntax.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:32 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:10:33 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_first_token(t_token *current)
{
	if (current && current->type == TYPE_PIPE)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token '|'\n");
		return (-1);
	}
	return (0);
}

int	check_intermediate_tokens(t_token *current)
{
	while (current)
	{
		if (current->type == TYPE_PIPE)
		{
			if (!current->next || current->next->type == TYPE_PIPE)
			{
				fprintf(stderr,
					"minishell: syntax error near unexpected token '|'\n");
				return (-1);
			}
		}
		if (current->type == TYPE_REDIR_IN || current->type == TYPE_REDIR_OUT
			|| current->type == TYPE_REDIR_APPEND)
		{
			if (!current->next || current->next->type != TYPE_WORD)
			{
				fprintf(stderr,
					"minishell: syntax error near unexpected token '%s'\n",
					current->value);
				return (-1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	check_last_token(t_token *tokens, t_token *current)
{
	if (tokens && current && (current->type == TYPE_PIPE
			|| current->type == TYPE_REDIR_IN
			|| current->type == TYPE_REDIR_OUT
			|| current->type == TYPE_REDIR_APPEND))
	{
		fprintf(stderr,
			"minishell: syntax errornear unexpected token 'newline'\n");
		return (-1);
	}
	return (0);
}

int	check_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (check_first_token(current) == -1)
		return (-1);
	if (check_intermediate_tokens(current) == -1)
		return (-1);
	while (current && current->next)
		current = current->next;
	if (check_last_token(tokens, current) == -1)
		return (-1);
	return (0);
}
