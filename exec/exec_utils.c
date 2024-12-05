/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:35:19 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/05 22:59:08 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_openin(t_cmd_list *list, char *fname)
{
	int	fd;

	(void)list;
	fd = open(fname, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}

int	ft_openout(t_cmd_list *list, char *fname)
{
	int	fd;

	(void)list;
	fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] && s1[i] == s2[i]))
		i++;
	return (*(unsigned char *)(s1 + i) - *(unsigned char *)(s2 + i));
}

int	ft_access(char **tab, char *avi, char **path)
{
	int		j;
	char	*temp;
	char	*templst;

	j = 0;
	templst = NULL;
	while (tab[j])
	{
		if (templst)
			free(templst);
		templst = ft_strdup(tab[j]);
		temp = ft_strjoin(tab[j], "/");
		free(tab[j]);
		tab[j] = ft_strjoin(temp, avi);
		free(temp);
		if (ft_access2(path, tab[j]) == 1)
			return (free(templst), 1);
		j++;
	}
	if (ft_access2(path, templst) == 1)
		return (free(templst), 1);
	free(templst);
	*path = NULL;
	perror("access");
	return (-1);
}

int	ft_access2(char **path, char *str)
{
	if (access(str, F_OK | X_OK) == 0)
	{
		*path = ft_strdup(str);
		return (1);
	}
	return (0);
}
