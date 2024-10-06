/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 20:35:19 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/06 23:26:40 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

int	openout(t_token *token)
{
	token->open = open(token->value ,O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (token->open == -1)
	{
		perror("open");
		return (-1);
	}
	return (0);
}

int     openin(t_token *token)
{
	token->open = open(token->value, O_RDONLY);
	if (token->open == -1)
	{
		perror("open");
		return (-1);
	}        
	return (0);
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
                        *path = tab[j];
                        return (1);
		}
		j++;
	}
	perror("access");
	return (-1);
}

char	**ft_get_path(char **envp)
{
	char	**tab;
	int		i;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
	{
		i++;
	}
	if (!envp[i])
		exit(-1);
	tab = ft_split(&envp[i][5], ':');
	if (!tab)
	{
		perror("ft_split");
		return (NULL);
	}
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


// t_env   *new_env_node(char *name, char *value)
// {
//     t_env *new_node;

//     new_node = (t_env *)malloc(sizeof(t_env));
//     if (!new_node)
//         return (NULL);
//     new_node->name = name;
//     new_node->value = value;
//     new_node->next = NULL;
//     return (new_node);
// }

// void    add_env_node(t_env **lst, t_env *new_node)
// {
//     t_env *tmp;

//     if (!*lst)
//     {
//         *lst = new_node;
//         return ;
//     }
//     tmp = *lst;
//     while (tmp->next)
//         tmp = tmp->next;
//     tmp->next = new_node;
// }

// t_env   *create_env_node(char *env_entry)
// {
//     char *delimiter;
//     char *name;
//     char *value;

//     delimiter = strchr(env_entry, '=');
//     if (!delimiter)
//         return (NULL);
//     name = strndup(env_entry, delimiter - env_entry);
//     value = strdup(delimiter + 1);
//     return (new_env_node(name, value));
// }

// t_env   *ft_envtolst(char **envp)
// {
//     t_env   *env_list;
//     t_env   *new_node;
//     int     i;

//     env_list = NULL;
//     i = 0;
//     while (envp[i])
//     {
//         new_node = create_env_node(envp[i]);
//         if (new_node)
//             add_env_node(&env_list, new_node);
//         i++;
//     }
//     return (env_list);
// }
