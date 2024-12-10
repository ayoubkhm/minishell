/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_assignments.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 03:05:16 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/10 12:42:44 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	extract_name_and_val(char *inp, t_variable_data *data)
{
	data->name = ft_substr(inp, data->start, data->val_start - data->start - 2);
	data->val = ft_substr(inp, data->val_start, data->end - data->val_start);
	if (!(data->name) || !(data->val))
	{
		free(data->name);
		free(data->val);
		return (-1);
	}
	return (0);
}

int	add_var_token_and_cleanup(t_variable_data *data, t_token **tokens, int i)
{
	int	result;

	result = add_variable_token(data->name, data->val, tokens, i);
	free(data->name);
	free(data->val);
	return (result);
}

int	parse_variable_assignment(char *input, int i, int start, t_token **tokens)
{
	t_variable_data	data;

	data.start = start;
	data.name = NULL;
	data.val = NULL;
	data.val_start = 0;
	data.end = 0;
	if (extract_indices(input, i, &data) || extract_name_and_val(input, &data))
	{
		return (-1);
	}
	return (add_var_token_and_cleanup(&data, tokens, data.end));
}
