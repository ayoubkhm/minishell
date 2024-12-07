/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_variables2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:04:09 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 03:09:29 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*initialize_var_value(char *inp, int i, t_env *e_l)
{
	char	var_name[2];
	char	*var_value;
	char	*token_value;

	var_name[0] = inp[i];
	var_name[1] = '\0';
	var_value = get_env_variable(e_l, var_name);
	if (var_value)
	{
		token_value = ft_strdup(var_value);
	}
	else
	{
		token_value = ft_strdup("");
	}
	return (token_value);
}

char	*accumulate_invalid_chars(char *inp, int *i, char *token_value)
{
	char	temp[2];
	char	*new_token;

	while (inp[*i] && !isspace(inp[*i])
		&& !ft_isalnum(inp[*i]) && inp[*i] != '_')
	{
		temp[0] = inp[*i];
		temp[1] = '\0';
		new_token = ft_strjoin(token_value, temp);
		free(token_value);
		token_value = new_token;
		(*i)++;
	}
	return (token_value);
}

int	finalize_and_add_tok(t_token **tok, char *tok_val, char *dol_seq, int i)
{
	add_token(tok, create_token(tok_val, TYPE_WORD, 0));
	free(tok_val);
	free(dol_seq);
	return (i);
}

int	handle_dollar_sequence(t_ctx *ctx)
{
	int	new_i;

	new_i = accumulate_dol_seq(ctx->inp, ctx->i, &ctx->dol_seq, &ctx->dol_cnt);
	if (new_i == -1)
	{
		return (-1);
	}
	ctx->i = new_i;
	return (0);
}
