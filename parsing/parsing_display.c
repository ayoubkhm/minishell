/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_display.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:20 by akhamass          #+#    #+#             */
/*   Updated: 2024/12/03 21:30:57 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token: %s   Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}
