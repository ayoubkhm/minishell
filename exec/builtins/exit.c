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

// long	ft_exatoi(t_cmd_list *list, int	*error)
// {
// 	long atoi;
// 	int s;
// 	char	*str;

// 	str = list->cmd_args[1];
// 	atoi = 0;
// 	s = 1;
// 	while ((*str >= 9 && *str <= 13) || (*str == ' '))
// 		str++;
// 	if (*str == '+' || *str == '-')
// 	{
// 		if (*str == '-')
// 			s = s * -1;
// 		str++;
// 	}
// 	while (*str >= '0' && *str <= '9')
// 	{
// 		atoi = atoi * 10 + (*str) - 48;
// 		str++;
// 		if ((s == 1 && atoi > LLONG_MAX) || (s == -1 && atoi * s < LLONG_MIN))
// 			return (2);
// 	}
// 	if (*str)
// 		return (2);
// 	else if(list->cmd_args[2])
// 		return(1);
// 	return (atoi * s);
// }

long	ft_exatoi(t_cmd_list *list)
{
	long	atoi;
	int		s;
	char	*str;

	str = list->cmd_args[1];
	atoi = 0;
	s = 1;
	while ((*str >= 9 && *str <= 13) || (*str == ' '))
		str++;
	if (*str == '+' || *str == '-')
		s = 1 - 2 * (*str++ == '-');
	while (*str >= '0' && *str <= '9')
	{
		atoi = atoi * 10 + (*str - '0');
		str++;
		if ((s == 1 && atoi > LLONG_MAX) || (s == -1 && atoi * s < LLONG_MIN))
			return (2);
	}
	if (*str)
		return (2);
	else if (list->cmd_args[2])
		return(1);
	return (atoi * s);
}


void	ft_exit(t_data *data,t_cmd_list *list)
{
	int i;

	write(1, "exit\n", 5);
	ft_freedata(data);
	if (list->cmd_args[1])
	{
		i = ft_exatoi(list);
		if(list->next)
			ft_free_list(list);
		else
			ft_free_inlist(list);
		if(i != 1 || !list->cmd_args[2])
			exit(i % 256);
		else
			perror("exit");
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
