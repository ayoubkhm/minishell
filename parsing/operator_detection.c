/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_detection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:18:22 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/27 12:18:31 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	detect_specific_oper(char *input, int i, char **oper_str, int *oper_type)
{
	if (input[i] == '>' && input[i + 1] == '>')
	{
		*oper_str = ">>";
		*oper_type = TYPE_REDIR_APPEND;
		return (2);
	}
	else if (input[i] == '>')
	{
		*oper_str = ">";
		*oper_type = TYPE_REDIR_OUT;
		return (1);
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		*oper_str = "<<";
		*oper_type = TYPE_HEREDOC;
		return (2);
	}
	else if (input[i] == '<')
	{
		*oper_str = "<";
		*oper_type = TYPE_REDIR_IN;
		return (1);
	}
	return (0);
}

int	detect_operator(char *input, int i, char **operator_str, int *operatortype)
{
	if (input[i] == '>' || input[i] == '<')
	{
		return (detect_specific_oper(input, i, operator_str, operatortype));
	}
	else if (input[i] == '|')
	{
		*operator_str = "|";
		*operatortype = TYPE_PIPE;
		return (1);
	}
	return (0);
}
