/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 01:04:24 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/18 17:12:06 by gtraiman         ###   ########.fr       */
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
char	**ft_tokentab(t_token *token);
int	tklast(t_token *token);
char	*rmstrbfc(char *str, char c);

int     ft_exec(t_token *token,t_data *data);
int     parsebi(t_token *token, t_data *data);
int     ft_exec2(t_token *token,t_data *data);
int	ft_checkarg(t_token *token, t_data *data);
char	*ft_get_command_path(char *cmd, t_data *data);
int	ft_is_absolute_path(char *cmd);


int	ft_rdrout(t_token *token, t_data *data);
int	ft_rdrin(t_token *token, t_data *data);
int	ft_rdrapp(t_token *token, t_data *data);

int	ft_execpipe(t_token *token,t_data *data);

int     ft_heredoc(t_token *token, t_data *data);

int     ft_envar(t_token *token,t_data *data);

#endif