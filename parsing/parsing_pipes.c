/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 12:10:26 by akhamass          #+#    #+#             */
/*   Updated: 2024/11/05 00:16:20 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_pipe(int i, t_token **tokens)
{
	add_token(tokens, create_token("|", TYPE_PIPE, 0));
	return (i + 1);
}
