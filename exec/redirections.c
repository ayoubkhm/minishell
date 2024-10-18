/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:29:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/18 13:59:15 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_rdrin(t_token *token, t_data *data)
{
	(void)data;
        if(!token->next)
                exit(2);
	if(token->next->type != 5 && token->next->type != 0 && token->next->type != 7)
		exit(2);
	if(openin(token->next) == -1)
		exit(1);
	exit(0);
	return(0);
}

int	ft_rdrout(t_token *token, t_data *data)
{
	(void)data;
        if(!token->next)
                exit(2);
	if(token->next->type != 5 && token->next->type != 0 && token->next->type != 7)
		exit(2);
	if(openout(token->next) == -1)
		exit(1);
	exit(0);
	return(0);
}

int	ft_rdrapp(t_token *token, t_data *data)
{
	(void)data;
        if(!token->next)
                exit(2);
	return(0);
}