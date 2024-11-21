/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 22:49:14 by gtraiman          #+#    #+#             */
/*   Updated: 2024/09/30 22:49:14 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builtins.h"


char	*ft_addstr(char *str1, char *str2)
{
	int i;
	char *stret;

	stret = malloc((strlen(str1) + strlen(str2) + 1) * sizeof(char));
	if (!stret)
		return (NULL);
	i = -1;
	while (str1[++i])
		stret[i] = str1[i];
	i = -1;
	while (str2[++i])
		stret[i + strlen(str1)] = str2[i];
	stret[i + strlen(str1)] = '\0';
	return (stret);
}

int	ft_tabstrlen(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	**ft_copytab(char **tab)
{
	char **tabret;
	int i;

	i = -1;
	tabret = malloc((ft_tabstrlen(tab) + 1) * sizeof(char *));
	if (!tabret)
		exit(1);
	while (tab[++i])
		tabret[i] = ft_strdup(tab[i]);
	tabret[i] = NULL;
	return (tabret);
}

char	**ft_copyntab(char **tab, int n)
{
	char **tabret;
	int i;

	tabret = NULL;
	i = -1;
	tabret = malloc((ft_tabstrlen(tab) + n + 1) * sizeof(char *));
	if (!tabret)
		exit(1);
	while (tab[++i])
		tabret[i] = ft_strdup(tab[i]);
	tabret[i] = NULL;
	return (tabret);
}

void	ft_freetab(char **tab)
{
	int i;

	if (!tab)
		return ;
	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

int	ft_printab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		write(1, tab[i], ft_strlen(tab[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}


void	ft_freedata(t_data *data)
{
	if (data->av)
		ft_freetab(data->av);
	if (data->envp)
		ft_freetab(data->envp);
	if(data->cwd)
		free(data->cwd);
}

void	ft_delfromc(char *str, char c)
{
	int		i;

	if (!str)
		return;
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	str[i+1] = '\0';
}

char	*rpstrfrmc(char *src,char *dest, char c)
{
	int	i;
	(void)dest;
	(void)c;
	i = 0;
	(void)i;
	return(src);
	
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
