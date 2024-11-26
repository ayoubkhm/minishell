/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/10/19 21:32:34 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/19 21:32:34 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"


long long	ft_exatoi(t_cmd_list *list)
{
	unsigned long long atoied;
	int s;
	char *str;

	str = list->cmd_args[1];
	atoied = 0;
	if (ft_strcmp(str, "-9223372036854775808") == 0)
		return (0);
	while ((*str >= 9 && *str <= 13) || (*str == ' '))
		str++;
	s = 1 - 2 * (*str == '-');
	if (*str == '+' || *str == '-')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		atoied = atoied * 10 + (*str - '0');
		str++;
		if (atoied > LLONG_MAX)
			return (list->error = 1, 2);
	}
	if (*str)
		return (list->error = 1, 2);
	else if (list->cmd_args[2])
		return (list->error = 2, 1);
	return (atoied * s);
}

void	ft_exit(t_data *data, t_env **env_list, t_cmd_list *list)
{
	long long exit_code;

	write(1, "exit\n", 5);
	exit_code = 0;
	list->error = 0;
	if (list->cmd_args[1])
	{
		exit_code = ft_exatoi(list);
		if (list->error == 1)
		{
			write(2, "exit: ", 6);
			write(2, list->cmd_args[1], ft_strlen(list->cmd_args[1]));
			write(2, ": numeric argument required\n", 29);
			cleanup_resources(data, env_list, list);
			exit(2);
		}
		if (list->error == 2)
		{
			write(2, "exit: too many arguments\n", 26);
			g_last_exit_status = 1;
			return ;
		}
		cleanup_resources(data, env_list, list);
		exit(exit_code % 256);
	}
	cleanup_resources(data, env_list, list);
	exit(g_last_exit_status);
}
