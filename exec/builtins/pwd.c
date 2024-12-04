/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/09/23 20:20:38 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/23 20:20:38 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_pwd(t_data *data, t_cmd_list *list)
{
	if (list->cmd_args[1] && list->cmd_args[1][0] == '-')
	{
		if (list->cmd_args[1][1] != 'P' && list->cmd_args[1][1] != 'L'
			&& list->cmd_args[1][2] != ' ')
		{
			write(2, "pwd: --: invalid option\n", 25);
			return (data->exit = 2, 2);
		}
	}
	printf("%s\n", data->cwd);
	return (0);
}
