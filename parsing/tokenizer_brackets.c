/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_brackets.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:04:28 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 03:04:29 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*extract_and_expand(char *input, int *i, t_env *env_list)
{
	int		start;
	char	*value;
	char	*expanded_value;

	start = ++(*i);
	while (input[*i] && input[*i] != ']')
		(*i)++;
	value = ft_strndup(&input[start], *i - start);
	expanded_value = expand_variables(value, env_list);
	free(value);
	return (expanded_value);
}

void	create_and_add_token(t_token **tok, char *xpndd_val, char is_closed)
{
	char	*tok_val;
	char	*temp;

	if (is_closed)
	{
		temp = ft_strjoin("[", xpndd_val);
		tok_val = ft_strjoin(temp, "]");
		free(temp);
	}
	else
		tok_val = ft_strjoin("[", xpndd_val);
	add_token(tok, create_token(tok_val, TYPE_WORD, 0));
	free(tok_val);
}

int	handle_brackets(char *input, int i, t_token **tokens, t_env *env_list)
{
	char	*expanded_value;
	int		is_closed;

	expanded_value = extract_and_expand(input, &i, env_list);
	if (input[i] == ']')
		is_closed = 1;
	else
		is_closed = 0;
	create_and_add_token(tokens, expanded_value, is_closed);
	free(expanded_value);
	if (is_closed)
		return (i + 1);
	else
		return (i);
}

t_token	*get_last_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens->next)
		tokens = tokens->next;
	return (tokens);
}

void	remove_last_token(t_token **tokens)
{
	t_token	*current;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	if (!current->next)
	{
		free(current->value);
		free(current);
		*tokens = NULL;
		return ;
	}
	while (current->next && current->next->next)
		current = current->next;
	free(current->next->value);
	free(current->next);
	current->next = NULL;
}
