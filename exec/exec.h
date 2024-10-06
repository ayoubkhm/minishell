/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 01:04:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/06 20:13:49 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "../minishell.h"


int     openin(t_token *token);
int     openout(t_token *token);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_access(char **tab, char *avi, char **path);
char	**ft_get_path(char **envp);
int	ft_strlkforc(char *str,char c);

int     ft_exec(t_token *token,t_data *data);
int     parsebi(t_token *token, t_data *data);
int     ft_exec2(t_token *token,t_data *data);

typedef struct s_env
{
    char            *name;
    char            *value;
    struct s_env    *next;
}                   t_env;


void    add_env_node(t_env **lst, t_env *new_node);
t_env   *create_env_node(char *env_entry);
t_env   *ft_envtolst(char **envp);

#endif