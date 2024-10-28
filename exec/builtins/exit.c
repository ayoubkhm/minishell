/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 21:32:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/19 21:32:34 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

long long	ft_exatoi(t_cmd_list *list)
{
	unsigned long long	atoi;
	int		s;
	char	*str;

	str = list->cmd_args[1];
	atoi = 0;
	if(ft_strcmp(str, "-9223372036854775808") == 0)
		return(0);
	while ((*str >= 9 && *str <= 13) || (*str == ' '))
		str++;
	if (*str == '+' || *str == '-')
		s = 1 - 2 * (*str++ == '-');
	while (*str >= '0' && *str <= '9')
	{
		atoi = atoi * 10 + (*str - '0');
		str++;
		if (atoi > LLONG_MAX)
			return(2);
	}
	if (*str)
		return(2);
	else if (list->cmd_args[2])
		return(1);
	return (atoi * s);
}


void	ft_exit(t_data *data,t_cmd_list *list)
{
	long long i;

	write(1, "exit\n", 5);
	ft_freedata(data);
	i = 0;
	if (list->cmd_args[1])
	{
		i = ft_exatoi(list);
		if(i != 1 || !list->cmd_args[2])
		{
			if(ft_isainnum(list->cmd_args[1]) == 1)
			{
				write(2,"exit: ",6);
				write(2,&list->cmd_args[1],1);
				write(2,": numeric argument required\n",29);
			}
			if(list->next)
				ft_free_list(list);
			else
				ft_free_inlist(list);
			exit(i % 256);
		}
		else 
			write(2,"exit: too many arguments\n",26);
	}
	else
	{
		(void)i;
		if(list->next)
			ft_free_list(list);
		else
			ft_free_inlist(list);
		exit(g_last_exit_status);  
	}
}
