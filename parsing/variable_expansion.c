/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:38:03 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:38:04 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*append_variable_value(char *value, t_var_exp *ctx)
{
	char	*var_value;
	char	*temp;

	var_value = handle_var_expansion(ctx);
	temp = ft_strjoin(value, var_value);
	if (!temp)
	{
		free(value);
		free(var_value);
		return (NULL);
	}
	free(value);
	free(var_value);
	return (temp);
}

char	*process_invalid_variable(t_var_exp *ctx, char *value)
{
	value = append_character(value, ctx->input[*ctx->index]);
	(*ctx->index)++;
	return (value);
}

char	*process_exit_status(t_var_exp *ctx, char *value)
{
	char	*exit_status_str;

	exit_status_str = handle_quoted_exit_status(ctx->env_list, ctx->index);
	if (!exit_status_str)
	{
		free(value);
		return (NULL);
	}
	return (append_and_free(value, exit_status_str));
}

char	*process_dollar_sign(t_var_exp *ctx, char *value)
{
	if (ctx->input[*ctx->index + 1] == '?')
		value = process_exit_status(ctx, value);
	else if (!is_valid_variable_start(ctx->input[*ctx->index + 1]))
		value = process_invalid_variable(ctx, value);
	else
		value = append_variable_value(value, ctx);
	return (value);
}

char	*handle_quoted_exit_status(t_env *env_list, int *index)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(env_list->exit_status);
	if (exit_status_str)
		(*index) += 2;
	return (exit_status_str);
}
