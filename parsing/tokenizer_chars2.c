/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:09:55 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 02:18:49 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_pars_cxt	init_pars_ctx(char *inp, int i, t_token **tok, t_env *env_list)
{
	t_pars_cxt	cxt;

	cxt.inp = NULL;
	cxt.i = 0;
	cxt.pfx = NULL;
	cxt.tok = NULL;
	cxt.e_l = NULL;
	cxt.inp = inp;
	cxt.i = i;
	cxt.pfx = NULL;
	cxt.tok = tok;
	cxt.e_l = env_list;
	return (cxt);
}

int	handle_dollar_character(t_pars_cxt *cxt)
{
	cxt->i = gere_var2(cxt);
	if (*cxt->tok == NULL || (*cxt->tok)->value == NULL)
	{
		free(cxt->pfx);
		cxt->pfx = NULL;
	}
	return (cxt->i);
}

void	handle_prefix(t_pars_cxt *cxt)
{
	if (cxt->pfx != NULL)
	{
		process_prefix_as_word(cxt->pfx, cxt->tok);
		free(cxt->pfx);
		cxt->pfx = NULL;
	}
}

int	handle_regular_characters(char *inp, int i, t_token **tok, t_env *env_list)
{
	t_pars_cxt	cxt;

	cxt = init_pars_ctx(inp, i, tok, env_list);
	cxt.i = handle_character_iteration(cxt.inp, cxt.i);
	cxt.pfx = extract_prefix(cxt.inp, i, cxt.i);
	if (cxt.inp[cxt.i] == '$')
		return (handle_dollar_character(&cxt));
	handle_prefix(&cxt);
	return (cxt.i);
}
