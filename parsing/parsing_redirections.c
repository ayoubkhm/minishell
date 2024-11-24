/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:30 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/05 00:15:56 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_redirection(char *input, int i, t_token **tokens)
{
	if (input[i] == '>' && input[i + 1] == '>')
	{
		add_token(tokens, create_token(">>", TYPE_REDIR_APPEND, 0)); // Pas d'expansion
		return (i + 2);
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		add_token(tokens, create_token("<<", TYPE_HEREDOC, 0)); // Pas d'expansion
		return (i + 2);
	}
	else if (input[i] == '>')
	{
		add_token(tokens, create_token(">", TYPE_REDIR_OUT, 0)); // Pas d'expansion
		return (i + 1);
	}
	else if (input[i] == '<')
	{
		add_token(tokens, create_token("<", TYPE_REDIR_IN, 0)); // Pas d'expansion
		return (i + 1);
	}
	return (i);
}
