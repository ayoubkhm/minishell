/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:04:49 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 03:04:50 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*extract_quoted_content(char *input, int *i, char quote_type)
{
	int		start;
	char	*quoted_content;

	(*i)++;
	start = *i;
	while (input[*i] != '\0' && input[*i] != quote_type)
	{
		(*i)++;
	}
	if (input[*i] == '\0')
	{
		return (NULL);
	}
	quoted_content = ft_substr(input, start, *i - start);
	(*i)++;
	return (quoted_content);
}

char	*xpnd_or_cp_cntnt(char *quoted_cntnt, char quote_type, t_env *e_l)
{
	char	*result;

	result = NULL;
	if (quote_type == '\'')
	{
		result = ft_strdup(quoted_cntnt);
	}
	else if (quote_type == '"')
	{
		result = expand_variables(quoted_cntnt, e_l);
	}
	return (result);
}

char	*safe_concat(char *final_content, char *new_content)
{
	char	*result;

	result = ft_strjoin(final_content, new_content);
	free(final_content);
	return (result);
}

char	*gere_adjcnt_txt(char *input, int *i, char *final_content)
{
	int		start;
	char	*adjacent_text;

	start = *i;
	adjacent_text = NULL;
	while (input[*i] && !isspace(input[*i]) && !is_operator(input[*i]))
	{
		(*i)++;
	}
	if (*i > start)
	{
		adjacent_text = ft_substr(input, start, *i - start);
		final_content = safe_concat(final_content, adjacent_text);
		free(adjacent_text);
	}
	return (final_content);
}

char	*process_quoted_section(t_quote_context *ctx, char quote_type)
{
	char	*quoted_cntnt;
	char	*xpndd_cntnt;

	quoted_cntnt = extract_quoted_content(ctx->input, ctx->i, quote_type);
	if (!quoted_cntnt)
	{
		free(ctx->final_content);
		return (NULL);
	}
	xpndd_cntnt = xpnd_or_cp_cntnt(quoted_cntnt, quote_type, ctx->env_list);
	free(quoted_cntnt);
	if (!xpndd_cntnt)
	{
		free(ctx->final_content);
		return (NULL);
	}
	ctx->final_content = safe_concat(ctx->final_content, xpndd_cntnt);
	free(xpndd_cntnt);
	return (ctx->final_content);
}
