/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:03:27 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 06:30:03 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_backslash(char *input, int i, char **current_value)
{
	char	escaped_char[2];
	char	literal_char[2];
	char	*temp;

	if (!input[i + 1])
	{
		return (-1);
	}
	escaped_char[0] = input[i + 1];
	escaped_char[1] = '\0';
	temp = *current_value;
	*current_value = ft_strjoin(*current_value, escaped_char);
	free(temp);
	i += 2;
	while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
	{
		literal_char[0] = input[i];
		literal_char[1] = '\0';
		temp = *current_value;
		*current_value = ft_strjoin(*current_value, literal_char);
		free(temp);
		i++;
	}
	return (i);
}

int	handle_character_iteration(char *input, int i)
{
	while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
			i++;
		else
			i++;
	}
	return (i);
}

char	*extract_prefix(char *input, int start, int i)
{
	if (start < i)
		return (ft_substr(input, start, i - start));
	return (NULL);
}

void	process_prefix_as_word(char *prefix, t_token **tokens)
{
	char	*cleaned_prefix;

	cleaned_prefix = NULL;
	if (prefix && *prefix)
	{
		cleaned_prefix = remove_quotes(prefix);
		free(prefix);
		add_token(tokens, create_token(cleaned_prefix, TYPE_WORD, 1));
		free(cleaned_prefix);
	}
}

int	handle_regular_characters(char *input, int i, t_token **tok, t_env *e_l)
{
	int			start;
	t_pars_cxt	ctx;

	start = i;
	ctx.inp = input;
	ctx.i = i;
	ctx.pfx = NULL;
	ctx.tok = tok;
	ctx.e_l = e_l;
	ctx.i = handle_character_iteration(ctx.inp, ctx.i);
	ctx.pfx = extract_prefix(ctx.inp, start, ctx.i);
	if (ctx.inp[ctx.i] == '$')
	{
		ctx.i = gere_var2(&ctx);
		i = ctx.i + 1;
	}
	else
	{
		process_prefix_as_word(ctx.pfx, tok);
		i = ctx.i;
	}
	return (i);
}
