/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <akhamass@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 23:23:01 by gtraiman          #+#    #+#             */
/*   Updated: 2024/12/08 00:38:33 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../../minishell.h"
# include "../../parsing/parsing.h"
# include <stdbool.h>

typedef struct s_data			t_data;
typedef struct s_env			t_env;
typedef struct s_cmd_list		t_cmd_list;

int		ft_cd(t_cmd_list *list, t_data *data);
int		ft_update_env(t_data *data, char *str);
int		ft_echo(char *str);
bool	is_n_option(char *arg);
int		ft_parsecho(t_cmd_list *list, t_data *data);
int		ft_env(char **envp);
void	ft_exit(t_data *data, t_env **env_list, t_cmd_list *list);
char	**ft_export(char **envp, char *str);
int		ft_parsexport(t_data *data, t_cmd_list *list);
int		ft_envexport(char **envp);
int		ft_pwd(t_data *data, t_cmd_list *list);
char	**ft_unset(char **envp, char *str);
int		ft_parsunset(t_data *data, t_cmd_list *list);

// UTILS
int		ft_dblexport(char **envp, char *str);
int		ft_checkexport(char *str);
char	*ft_addstr(char *str1, char *str2);
void	ft_freedata(t_data *data);
void	ft_freetab(char **tab);
void	ft_freetabi(char **tab, int i);
char	**ft_copytab(char **tab);
char	**ft_copyntab(char **tab, int n);
int		ft_printab(char **tab);
int		ft_tabstrlen(char **tab);
void	ft_delfromc(char *str, char c);
int		ft_isainnum(char *str);

#endif