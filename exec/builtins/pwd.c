/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 20:20:38 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/23 20:20:38 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_pwd(t_data *data, t_cmd_list *list)
{
	int	i;

	i = 0;
	if(list->cmd_args[1] && list->cmd_args[1][0] == '-')
	{
		if(list->cmd_args[1][1] != 'P' && list->cmd_args[1][1] != 'L' && list->cmd_args[1][2] != ' ')
		{
			// write(1,"pwd: --: invalid option",24);
			return(g_last_exit_status = 2, 2);
		}
	}
	while(data->cwd[i])
	{
		write(1,&data->cwd[i],1);
		i++;
	}
	write(1,"\n",1);
	return (0);
}
