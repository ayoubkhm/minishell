/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:35:19 by gtraiman          #+#    #+#             */
/*   Updated: 2024/11/21 21:23:04 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

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

	j = 0;
	while (tab[j])
	{
		temp = ft_strjoin(tab[j], "/");
		free(tab[j]);
		tab[j] = ft_strjoin(temp, avi);
		free(temp);
		if (access(tab[j], F_OK | X_OK) == 0)
		{
			*path = ft_strdup(tab[j]);
			return (1);
		}
		j++;
	}
	*path = NULL;
	perror("access");
	return (-1);
}

char	**ft_get_path(char **envp)
{
	char	**tab;
	int		i;
	char **split;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
	{
		i++;
	}
	if (!envp[i])
		return(NULL); ///check leaks
	split = ft_split(&envp[i][5], ':');
	tab = ft_copyntab(split,1);
	if (!tab)
	{
		perror("ft_split");
		return (NULL);
	}
	i = 0;
	while(tab[i])
		i++;
	tab[i] = malloc(1024);
	if (!tab[i])
	{
		perror("malloc");
		return (NULL);
	}
	if (!getcwd(tab[i], 1024))
	{
		perror("getcwd");
		free(tab[i]);
		tab[i] = NULL;
	}
	tab[i + 1] = NULL;
	ft_freetab(split);
	return (tab);
}

int	ft_strlkforc(char *str,char c)
{
	while(*str)
	{
		if(*str == c)
			return(1);
		str++;
	}
	return (0);
}

char	*rmstrbfc(char *str, char c)
{
	int	i;
	char	*str2;
	int	j;

	j = -1;
	i = 0;
	while(str[i] && str[i] != c)
		i++;
	if(!str)
		return(ft_strdup(str));
	str2 = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if(!str2)
		return(NULL);
	while(str[++i])
		str2[j++] = str[i];
	free(str);
	str2[j] = '\0';
	return(str2);
}

void	ft_free_list(t_cmd_list *list)
{
	t_cmd_list *temp;
	while(list)
	{
		temp = list->next;
		ft_free_inlist(list);
		list = temp;
	}
}

void	ft_free_inlist(t_cmd_list *list)
{	
	ft_freetab(list->files_list);
	free(list->files_type);
	ft_freetab(list->cmd_args);
	free(list->cmd);
	free(list);
}