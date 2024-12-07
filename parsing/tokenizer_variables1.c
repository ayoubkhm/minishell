/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_variables1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 05:28:20 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 17:46:00 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	gere_var2(t_pars_cxt *ctx)
{
	char	*var_value;
	char	*combined;

	var_value = NULL;
	combined = NULL;

	// Appel de gere_var_ref pour récupérer la valeur de la variable
	ctx->i = gere_var_ref(ctx->inp, ctx->i, ctx->tok, ctx->e_l);

	// Si la variable est invalide ou vide
	if (*(ctx->tok) == NULL || (*(ctx->tok))->value == NULL)
	{
		free(ctx->pfx); // Libère le préfixe
		ctx->pfx = NULL;
		return (ctx->i);
	}

	// Récupérer la valeur de la variable
	var_value = (*(ctx->tok))->value;

	// Si un préfixe existe, créer une nouvelle combinaison
	if (ctx->pfx && *(ctx->pfx))
	{
		combined = ft_strjoin(ctx->pfx, var_value); // Combinaison préfixe + valeur
		free(ctx->pfx); // Libère le préfixe
		ctx->pfx = NULL; // Préfixe traité
		free(var_value); // Libère l'ancienne valeur de la variable
		(*(ctx->tok))->value = combined; // Mise à jour du token avec la valeur combinée
	}
	else
	{
		// Si aucun préfixe, la valeur de la variable reste telle quelle
		(*(ctx->tok))->value = var_value;
	}

	// Retourne l'indice mis à jour
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
	if (ctx->inp[ctx->i] && (ft_isalnum(ctx->inp[ctx->i])
			|| ctx->inp[ctx->i] == '_'))
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
