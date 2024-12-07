/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 02:37:07 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 02:37:08 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	parse_input(char *input, t_token **tokens, t_env *env_list)
{
	int	i;
	int	new_i;

	i = 0;
	new_i = 0;
	while (input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue ;
		}
		new_i = parse_word(input, i, tokens, env_list);
		if (new_i == -1)
			return (-1);
		i = new_i;
	}
	return (0);
}

int	parse_word(char *input, int i, t_token **tokens, t_env *env_list)
{
	char	*value;
	char	*new_value;
	char	*temp;

	temp = NULL;
	value = ft_strdup("");
	if (!value)
		return (-1);
	while (input[i] && !isspace(input[i]))
	{
		temp = parse_character(input, &i, tokens, env_list);
		if (!temp)
		{
			free(value);
			return (-1);
		}
		new_value = join_and_free(value, temp);
		if (!new_value)
			return (-1);
		value = new_value;
	}
	add_token(tokens, create_token(value, TYPE_WORD, 1));
	free(value);
	return (i);
}

char	*parse_character(char *input, int *i, t_token **tokens, t_env *env_list)
{
	char	*temp;

	temp = NULL;
	if (input[*i] == '\'')
		temp = handle_single_quotes(input, i, env_list);
	else if (input[*i] == '"')
		temp = gere_duo_quote(input, i, tokens, env_list);
	else
	{
		temp = ft_substr(input, *i, 1);
		(*i)++;
	}
	return (temp);
}

char	*handle_single_quotes(char *input, int *i, t_env *env_list)
{
	int		start;
	int		j;
	char	*quoted_part;

	start = *i + 1;
	j = start;
	quoted_part = NULL;
	while (input[j] && input[j] != '\'')
	{
		j++;
	}
	if (input[j] != '\'')
	{
		env_list->exit_status = 127;
		return (NULL);
	}
	quoted_part = ft_substr(input, start, j - start);
	if (!quoted_part)
	{
		return (NULL);
	}
	*i = j + 1;
	return (quoted_part);
}

char	*gere_duo_quote(char *input, int *i, t_token **tokens, t_env *env_list)
{
	t_var_exp			ctx;
	t_var_exp_params	params;
	char				*value;

	params.input = input;
	params.index = i;
	params.tokens = tokens;
	params.env_list = env_list;
	if (!init_var_exp_struc(&ctx, &params))
		return (NULL);
	value = process_double_quote_content(&ctx);
	if (!value || !is_valid_closing_quote(input, i, env_list, value))
		return (NULL);
	return (value);
}
