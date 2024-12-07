/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_extraction.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:11:07 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 19:09:58 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*extract_variable_value(char *input, int *i, t_env *env_list)
{
	char	*var_name;
	char	*var_value;
	char	*var_value_copy;
	int		var_start;

	var_name = NULL;
	var_value = NULL;
	var_value_copy = NULL;
	var_start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		(*i)++;
	}
	var_name = ft_substr(input, var_start, *i - var_start);
	if (!var_name)
		return (NULL);
	var_value = get_env_variable(env_list, var_name);
	free(var_name);
	if (!var_value)
		var_value_copy = ft_strdup("");
	else
		var_value_copy = ft_strdup(var_value);
	if (!var_value_copy)
		return (NULL);
	return (var_value_copy);
}

char	**xtrct_split_var_val(char *inp, int *i, t_env *e_l, char *dol_seq)
{
	char	*var_value;
	char	**split_values;

	var_value = extract_variable_value(inp, i, e_l);
	if (!var_value || var_value[0] == '\0')
	{
		free(var_value);
		free(dol_seq);
		return (NULL);
	}
	split_values = ft_split(var_value, ' ');
	free(var_value);
	if (!split_values)
	{
		free(dol_seq);
		return (NULL);
	}
	return (split_values);
}

char	*bild_cmbined_val(int dol_cnt, char *dol_seq, char *var_val)
{
	char	*temp;
	char	*combined_value;

	if (dol_cnt > 1)
		temp = ft_substr(dol_seq, 0, dol_cnt - 1);
	else
		temp = ft_strdup("");
	if (!temp)
		return (NULL);
	combined_value = ft_strjoin(temp, var_val);
	free(temp);
	return (combined_value);
}

int	append_special_chars(char *input, int i, char **combined_value)
{
	int		j;
	char	*special_chars;
	char	*temp;

	j = i;
	special_chars = NULL;
	temp = NULL;
	while (input[j] && !isspace(input[j])
		&& !is_operator(input[j]) && !ft_isalnum(input[j]))
	{
		j++;
	}
	if (j > i)
	{
		special_chars = ft_substr(input, i, j - i);
		if (!special_chars)
			return (i);
		temp = *combined_value;
		*combined_value = ft_strjoin(*combined_value, special_chars);
		free(temp);
		free(special_chars);
		return (j);
	}
	return (i);
}

void	free_split_values(char **split_values)
{
	int	j;

	j = 0;
	while (split_values[j])
	{
		free(split_values[j]);
		j++;
	}
	free(split_values);
}
