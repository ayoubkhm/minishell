/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tokenizer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:09:15 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 17:45:04 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	process_token(char *input, int i, t_token **tokens, t_env *env_list)
{
	int	result;

	result = proc_special_tok(input, i, tokens, env_list);
	if (result != -1)
		return (result);
	result = proc_complex_tok(input, i, tokens, env_list);
	if (result != -1)
		return (result);
	return (handle_word(input, i, tokens, env_list));
}

t_token	*tokenize_input(char *input, t_env *env_list)
{
	int		i;
	t_token	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue ;
		}
		i = process_token(input, i, &tokens, env_list);
		if (i == -1)
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}

int	handle_word(char *input, int i, t_token **tokens, t_env *env_list)
{
	int	start;

	start = i;
	while (input[i] && !isspace(input[i]) && input[i] != '=')
	{
		i++;
	}
	if (input[i] == '=' && input[i + 1] == '"')
	{
		return (parse_variable_assignment(input, i, start, tokens));
	}
	else
	{
		return (handle_regular_characters(input, start, tokens, env_list));
	}
}

int	handle_quotes(t_ctx *ctx)
{
	int	result;

	result = check_and_gere_quotes(ctx->inp, ctx->i, ctx->tok, ctx->e_l);
	if (result != -1)
	{
		return (result);
	}
	return (-1);
}

int	gere_var_ref(char *inp, int i, t_token **tok, t_env *e_l)
{
	t_ctx	ctx;
	int		result;

	ctx.inp = inp;
	ctx.i = i;
	ctx.tok = tok;
	ctx.e_l = e_l;
	ctx.dol_seq = NULL;
	ctx.dol_cnt = 0;
	result = handle_quotes(&ctx);
	if (result != -1)
		return (result);
	if (handle_dollar_sequence(&ctx) == -1)
		return (-1);
	result = handle_variable_type(&ctx);
	if (result != -1)
		return (result);
	return (handle_invalid_variable(&ctx));
}
