/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 01:42:20 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:44:09 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*expand_variable(char *arg, int *i, t_env *env_list, char *result)
{
	int		var_start;
	char	*var_name;
	char	*var_value;
	char	*temp;

	*i = *i + 1;
	if (!arg[*i] || !(isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
		return (append_character_main(result, '$'));
	var_start = *i;
	while (arg[*i] && (isalnum((unsigned char)arg[*i]) || arg[*i] == '_'))
	{
		*i = *i + 1;
	}
	var_name = ft_strndup(&arg[var_start], *i - var_start);
	var_value = get_env_variable(env_list, var_name);
	if (var_value)
	{
		temp = result;
		result = ft_strjoin(result, var_value);
		free(temp);
	}
	free(var_name);
	return (result);
}

char	*append_character_main(char *result, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = result;
	result = ft_strjoin(result, temp_str);
	free(temp);
	return (result);
}

char	*append_string(char *original, char *addition)
{
	char	*temp;

	temp = ft_strjoin(original, addition);
	free(original);
	return (temp);
}
