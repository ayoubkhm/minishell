/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 23:17:45 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/04 23:22:04 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	ft_delfromc(char *str, char c)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	str[i + 1] = '\0';
}

char	*getstrfrmc(char *str, char c)
{
	char	*newstr;
	int		i;
	int		len;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (!str[i])
		return (NULL);
	len = 0;
	while (str[i + len])
		len++;
	newstr = malloc(sizeof(char) * (len + 1));
	if (!newstr)
		return (NULL);
	i++;
	len = 0;
	while (str[i])
		newstr[len++] = str[i++];
	newstr[len] = '\0';
	return (newstr);
}

int	ft_isainnum(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '-' || str[i] == '+') && i != 0)
			return (1);
		if ((str[i] < '0' || str[i] > '9') && str[i] != '-' && str[i] != '+')
			return (1);
		i++;
	}
	return (0);
}

int	ft_checkexport(char *str)
{
	int	i;

	if (!str || !str[0])
		return (1);
	if (!ft_isalpha(str[0]))
		return (1);
	i = 1;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i] && str[i] != '=')
		return (1);
	return (0);
}
