/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:36 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:37:37 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*join_and_free(char *value, char *temp)
{
	char	*new_value;

	new_value = ft_strjoin(value, temp);
	free(value);
	free(temp);
	return (new_value);
}

char	*append_character(char *value, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = ft_strjoin(value, temp_str);
	free(value);
	return (temp);
}

char	*append_and_free(char *value, char *to_append)
{
	char	*new_value;

	new_value = ft_strjoin(value, to_append);
	free(value);
	free(to_append);
	return (new_value);
}

int	is_valid_closing_quote(char *input, int *i, t_env *env_list, char *value)
{
	if (input[*i] != '"')
	{
		env_list->exit_status = 127;
		free(value);
		return (0);
	}
	(*i)++;
	return (1);
}

int	is_valid_variable_start(char c)
{
	return (isalnum((unsigned char)c) || c == '_');
}
