/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 00:05:59 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 00:13:42 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_iscinstr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

int	ft_strlkforc(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

char	*rmstrbfc(char *str, char c)
{
	int		i;
	char	*str2;
	int		j;

	j = -1;
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	if (!str)
		return (ft_strdup(str));
	str2 = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if (!str2)
		return (NULL);
	while (str[++i])
		str2[j++] = str[i];
	free(str);
	str2[j] = '\0';
	return (str2);
}

void	ft_free_list(t_cmd_list *list)
{
	t_cmd_list	*temp;

	while (list)
	{
		temp = list->next;
		ft_free_inlist(list);
		list = temp;
	}
}
