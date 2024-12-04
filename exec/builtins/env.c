/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 19:45:58 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/23 19:45:58 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp || !envp[i])
		return (1);
	while (envp[i])
	{
		if(ft_iscinstr(envp[i],'=') == 1)
			printf("%s\n",envp[i]);
		i++;
	}
	return (0);
}