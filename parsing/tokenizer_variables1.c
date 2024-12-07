/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_variables1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 05:28:20 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 20:44:07 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	gere_var2(t_pars_cxt *ctx)
{
    char	*combined;

    combined = NULL;


    ctx->i = gere_var_ref(ctx->inp, ctx->i, ctx->tok, ctx->e_l);

    if (*(ctx->tok) == NULL || (*(ctx->tok))->value == NULL)
    {
        free(ctx->pfx);
        ctx->pfx = NULL;
        return (ctx->i);
    }
    if (ctx->pfx && *(ctx->pfx))
    {
        t_token *token = *(ctx->tok);
        t_token *first_var_token = NULL;
        while (token)
        {
            if (token->expand == 1)
            {
                first_var_token = token;
                break ;
            }
            token = token->next;
        }
        if (first_var_token)
        {
            combined = ft_strjoin(ctx->pfx, first_var_token->value);
            free(ctx->pfx);
            ctx->pfx = NULL;
            free(first_var_token->value);
            first_var_token->value = combined;
            first_var_token->expand = 0;

        }
    }
	//     token = *(ctx->tok);
//     while (token)
//     {
//         if (token->expand != 0)
//         {
//             token->expand = 0;
//         }
//         token = token->next;
//     }
    return (ctx->i);
}





int	handle_variable_type(t_ctx *ctx)
{
	int	result;

	if (ctx->inp[ctx->i] == '?')
	{
		result = gere_spcial_var(ctx);
		return (result);
	}
	if (ctx->inp[ctx->i] && ft_isdigit(ctx->inp[ctx->i]))
	{
		result = gere_num_var(ctx);
		return (result);
	}
	if (ctx->inp[ctx->i] && (ft_isalnum(ctx->inp[ctx->i]) || ctx->inp[ctx->i] == '_'))
	{
		result = gere_valid_var(ctx);
		return (result);
	}
	return (-1);
}


char	*initialize_exit_status(t_ctx *ctx)
{
	char	*exit_status;
	char	*token_value;

	exit_status = ft_itoa(ctx->e_l->exit_status);
	if (!exit_status)
		return (NULL);
	token_value = ft_strdup(exit_status);
	free(exit_status);
	return (token_value);
}

char	*accumulate_variable_value(t_ctx *ctx, char *token_value)
{
	char	temp[2];
	char	*new_token;

	while (ctx->inp[ctx->i + 1]
		&& !isspace(ctx->inp[ctx->i + 1])
		&& !is_operator(ctx->inp[ctx->i + 1]))
	{
		temp[0] = ctx->inp[++(ctx->i)];
		temp[1] = '\0';
		new_token = ft_strjoin(token_value, temp);
		free(token_value);
		token_value = new_token;
		if (!token_value)
			return (NULL);
	}
	return (token_value);
}

int	gere_spcial_var(t_ctx *ctx)
{
	char	*token_value;

	token_value = initialize_exit_status(ctx);
	if (!token_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	token_value = accumulate_variable_value(ctx, token_value);
	if (!token_value)
	{
		free(ctx->dol_seq);
		return (-1);
	}
	add_token(ctx->tok, create_token(token_value, TYPE_WORD, 0));
	free(token_value);
	free(ctx->dol_seq);
	return (ctx->i + 1);
}
