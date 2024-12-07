/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_chars3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 23:10:50 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/07 23:10:52 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h" 

int	handle_solo_quotes(char *input, int i, int *in_single_quotes)
{
	(void)input;
	*in_single_quotes = 0;
	i++;
	return (i);
}

int	handle_double_quotes(char *input, int i, int *in_double_quotes)
{
	(void)input;
	*in_double_quotes = 0;
	i++;
	return (i);
}

int	gere_out_quote(char *input, int i, int *in_sing_quot, int *in_duo_quot)
{
	if (isspace((unsigned char)input[i])
		|| is_operator(input[i]) || input[i] == '$')
		return (-1);
	if (input[i] == '\'')
	{
		*in_sing_quot = 1;
		i++;
		return (i);
	}
	if (input[i] == '"')
	{
		*in_duo_quot = 1;
		i++;
		return (i);
	}
	i++;
	return (i);
}

int	handle_character_iteration(char *input, int i)
{
	int	in_single_quotes;
	int	in_double_quotes;
	int	result;

	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
	{
		if (in_single_quotes)
		{
			i = handle_solo_quotes(input, i, &in_single_quotes);
			continue ;
		}
		else if (in_double_quotes)
		{
			i = handle_double_quotes(input, i, &in_double_quotes);
			continue ;
		}
		result = gere_out_quote(input, i, &in_single_quotes, &in_double_quotes);
		if (result == -1)
			break ;
		i = result;
	}
	return (i);
}
