/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:12:26 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 01:42:15 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_exit_status(t_env *env_list, char **result)
{
	char	*exit_status;
	char	*temp;

	exit_status = ft_itoa(env_list->exit_status);
	if (!exit_status)
	{
		free(*result);
		return (-1);
	}
	temp = *result;
	*result = ft_strjoin(temp, exit_status);
	free(temp);
	free(exit_status);
	if (!(*result))
	{
		return (-1);
	}
	return (0);
}

int	append_character_main_and_check(char **result, char c)
{
	char	*temp;

	temp = append_character_main(*result, c);
	if (!temp)
	{
		free(*result);
		return (-1);
	}
	*result = temp;
	return (0);
}

int	expand_variable_and_check(char *arg, int *i, t_env *env_list, char **result)
{
	char	*temp;

	temp = expand_variable(arg, i, env_list, *result);
	if (!temp)
	{
		free(*result);
		return (-1);
	}
	*result = temp;
	return (0);
}

int	handle_special_case(char *arg, int *i, t_env *env_list, char **result)
{
	if (arg[*i + 1] == '?')
	{
		if (handle_exit_status(env_list, result) == -1)
			return (-1);
		*i += 2;
	}
	else if (!arg[*i + 1]
		|| !(isalnum((unsigned char)arg[*i + 1]) || arg[*i + 1] == '_'))
	{
		if (append_character_main_and_check(result, arg[(*i)++]) == -1)
			return (-1);
	}
	else
	{
		if (expand_variable_and_check(arg, i, env_list, result) == -1)
			return (-1);
	}
	return (0);
}

char	*expand_variables(char *arg, t_env *env_list)
{
	char	*result;
	int		i;

	result = strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
		{
			if (handle_special_case(arg, &i, env_list, &result) == -1)
				return (NULL);
		}
		else
		{
			if (append_character_main_and_check(&result, arg[i++]) == -1)
				return (NULL);
		}
	}
	return (result);
}
